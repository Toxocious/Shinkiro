import * as cheerio from 'cheerio';
import * as fs from 'fs/promises';
import * as path from 'path';
import { Worker } from 'worker_threads';
import * as os from 'os';

import axios from 'axios';
import AdmZip from 'adm-zip';

import { PopulateModelSourceDirectoryArray } from './PopulateModelSources';
import { PrefetchAssetUrls } from './PrefetchAssetUrls';

const args = process.argv.slice(2);

const shouldDownload = {
    models: args.includes('--models'),
    audio: args.includes('--audio'),
    sprites: args.includes('--sprites'),
    extract: args.includes('--extract'),
};

if (!shouldDownload.models && !shouldDownload.audio && !shouldDownload.sprites) {
    shouldDownload.models = true;
    shouldDownload.audio = true;
    shouldDownload.sprites = true;
    shouldDownload.extract = false;
}

interface WorkerResult {
    completedCount: number;
    downloadedFiles: string[];
}

type AssetSource = {
    url: string;
    outputDir: string;
    fileType?: string;
};

export type SourceDirectoryMap = {
    models: AssetSource[];
    audio: AssetSource[];
    sprites: AssetSource[];
};

export function getErrorMessage(error: unknown): string {
    if (error instanceof Error) return error.message;
    return String(error);
}

async function findDirRecursive(startPath: string, dirName: string): Promise<string | null> {
    try {
        const items = await fs.readdir(startPath, { withFileTypes: true });
        for (const item of items) {
            const currentPath = path.join(startPath, item.name);
            if (item.isDirectory()) {
                if (item.name.toLowerCase() === dirName.toLowerCase()) {
                    return currentPath;
                }
                const result = await findDirRecursive(currentPath, dirName);
                if (result) {
                    return result;
                }
            }
        }
    } catch (e) {
        // Ignore errors like permission denied and continue search
    }
    return null;
}

async function downloadAssets() {
    const sourceDirectories: SourceDirectoryMap = {
        // 3D Pokemon Models
        models: [],

        // Pokemon Cries
        audio: [
            {
                url: 'https://play.pokemonshowdown.com/audio/cries/src/',
                outputDir: './Audio/Cries',
                fileType: '.wav',
            },
        ],

        // Pokemon Sprites, Icons, and Item Images
        sprites: [
            {
                url: 'https://play.pokemonshowdown.com/sprites/gen5/',
                outputDir: './Sprites/Normal',
                fileType: '.png',
            },
            {
                url: 'https://play.pokemonshowdown.com/sprites/gen5-back/',
                outputDir: './Sprites/Normal-Backs',
                fileType: '.png',
            },
            {
                url: 'https://play.pokemonshowdown.com/sprites/gen5-shiny/',
                outputDir: './Sprites/Shiny',
                fileType: '.png',
            },
            {
                url: 'https://play.pokemonshowdown.com/sprites/gen5-back-shiny/',
                outputDir: './Sprites/Shiny-Backs',
                fileType: '.png',
            },
            {
                url: 'https://play.pokemonshowdown.com/sprites/itemicons/',
                outputDir: './Sprites/Items',
                fileType: '.png',
            },
            {
                url: 'https://play.pokemonshowdown.com/sprites/categories/',
                outputDir: './Sprites/Categories',
                fileType: '.png',
            },
            {
                url: 'https://play.pokemonshowdown.com/sprites/types/',
                outputDir: './Sprites/Types',
                fileType: '.png',
            },
        ],
    };

    const NUM_WORKERS = Math.max(1, os.cpus().length - 1);

    for (const sourceType of Object.keys(sourceDirectories) as (keyof SourceDirectoryMap)[]) {
        if (!shouldDownload[sourceType]) {
            console.log(`\n [~] Skipping ${sourceType} downloads due to CLI flags...`);
            continue;
        }

        console.log(`\nProcessing ${sourceType} sources...`);

        if (sourceType === 'models') {
            PopulateModelSourceDirectoryArray(sourceDirectories);
        }

        // There are no sources for this type.
        const sources = sourceDirectories[sourceType];
        if (sources.length === 0) {
            console.log(`No sources found for ${sourceType}. Skipping...`);
            continue;
        }

        let fileUrls: string[] = [];
        let baseUrl = '';

        // Process model resources
        if (sourceType === 'models') {
            fileUrls = await PrefetchAssetUrls(sourceDirectories);
            baseUrl = 'https://www.models-resource.com/';

            const source = sources[0];
            await fs.mkdir(source.outputDir, { recursive: true });
        }
        // Process audio and sprite resources
        else {
            for (const source of sources) {
                await fs.mkdir(source.outputDir, { recursive: true });

                console.log(`Fetching file list from: ${source.url}`);

                try {
                    const response = await axios.get(source.url, {
                        responseType: 'text',
                        headers: {
                            'User-Agent': 'Mozilla/5.0',
                        },
                    });

                    const $ = cheerio.load(response.data);
                    const hrefs = $('a')
                        .map((i, link) => $(link).attr('href'))
                        .get()
                        .filter(
                            (href): href is string =>
                                !!href &&
                                source.fileType !== undefined &&
                                href.endsWith(source.fileType)
                        );

                    const fullUrls = hrefs.map((href) => new URL(href, source.url).href);
                    fileUrls.push(...fullUrls);

                    baseUrl = source.url;
                } catch (err) {
                    console.error(
                        `Failed to fetch list from ${source.url}: ${getErrorMessage(err)}`
                    );
                    continue;
                }
            }
        }

        /**
         * Process all found source files.
         */
        for (const source of sources) {
            console.log(` - Source URL: ${source.url}`);
        }

        console.log(`[~] Beginning downloads for ${sourceType}`);

        if (fileUrls.length === 0) {
            console.log(`No files of were found for ${sourceType}. Skipping.`);
            continue;
        }

        console.log(`Distributing ${fileUrls.length} files among ${NUM_WORKERS} workers...`);

        const filesPerWorker = Math.ceil(fileUrls.length / NUM_WORKERS);
        const workerPromises = [];
        let workersCompleted = 0;

        for (let i = 0; i < NUM_WORKERS; i++) {
            const start = i * filesPerWorker;
            const end = start + filesPerWorker;
            const workerFiles = fileUrls.slice(start, end);

            if (workerFiles.length === 0) continue;

            const outputDir = sources[0].outputDir;

            const workerPromise = new Promise((resolve, reject) => {
                const worker = new Worker(path.resolve(__dirname, 'Asset_Worker.js'), {
                    workerData: {
                        spriteUrls: workerFiles,
                        baseUrl,
                        outputDir,
                        workerId: i + 1,
                        sourceDir: outputDir,
                        headers: {
                            'User-Agent': 'Mozilla/5.0',
                            'Referer': 'https://www.models-resource.com/',
                        },
                    },
                });

                worker.on('message', (msg) => {
                    if (msg.type === 'progress') {
                        process.stdout.write(
                            `\rWorker ${msg.workerId} for ${path.basename(msg.sourceDir)}: ${
                                msg.completed
                            }/${msg.total} downloaded...`
                        );
                    } else if (msg.type === 'complete') {
                        workersCompleted++;
                        if (workersCompleted === NUM_WORKERS) {
                            process.stdout.write('\n');
                        }
                        resolve({
                            completedCount: msg.completedCount,
                            downloadedFiles: msg.downloadedFiles,
                        });
                    } else if (msg.type === 'error') {
                        console.error(`\nWorker ${msg.workerId} error: ${msg.error}`);
                        reject(new Error(msg.error));
                    }
                });

                worker.on('error', reject);
                worker.on('exit', (code) => {
                    if (code !== 0) {
                        reject(new Error(`Worker stopped with exit code ${code}`));
                    }
                });
            });

            workerPromises.push(workerPromise);
        }

        const results = (await Promise.all(workerPromises)) as WorkerResult[];
        console.log(results);
        const totalDownloaded = results.reduce((acc, result) => acc + result.completedCount, 0);

        console.log(
            `Finished downloading ${totalDownloaded} files for ${sourceType} into ${sources[0].outputDir}`
        );

        // extracting models
        // --- Extraction Logic ---
        if (sourceType === 'models' && shouldDownload.extract) {
            console.log('Extracting model archives...');
            console.log('Extracting and processing model archives...');
            const finalOutputDir = './Models_Extracted';
            await fs.mkdir(finalOutputDir, { recursive: true });

            let extractedCount = 0;

            for (const result of results) {
                for (const filePath of result.downloadedFiles) {
                    if (filePath.endsWith('.zip')) {
                        const parentDir = path.dirname(filePath);
                        const zipFileName = path.basename(filePath, '.zip');
                        const tempExtractDir = path.join(parentDir, zipFileName);

                        try {
                            await fs.mkdir(tempExtractDir, { recursive: true });
                            const zip = new AdmZip(filePath);
                            zip.extractAllTo(tempExtractDir, true);

                            const imagesDirPath = await findDirRecursive(tempExtractDir, 'images');

                            if (imagesDirPath) {
                                const targetDir = path.dirname(imagesDirPath);
                                const targetDirName = path.basename(targetDir);
                                const finalDestinationPath = path.join(
                                    finalOutputDir,
                                    targetDirName
                                );

                                await fs.rename(targetDir, finalDestinationPath);
                                extractedCount++;
                            } else {
                                console.warn(
                                    `\nWarning: 'images' directory not found in ${zipFileName}.`
                                );
                            }

                            // Cleanup
                            // await fs.unlink(filePath);
                            // await fs.rm(tempExtractDir, { recursive: true, force: true });
                        } catch (err) {
                            console.error(
                                `\nFailed to process ${zipFileName}:`,
                                getErrorMessage(err)
                            );
                            try {
                                await fs.rm(tempExtractDir, { recursive: true, force: true });
                            } catch (cleanupErr) {
                                // Ignore cleanup error
                            }
                        }
                    }
                }
            }
            console.log(`Extraction complete. ${extractedCount} archives extracted.`);
        }
    }

    return;
}

downloadAssets();
