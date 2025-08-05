import * as cheerio from 'cheerio';
import * as fs from 'fs/promises';
import * as path from 'path';
import { Worker } from 'worker_threads';
import * as os from 'os';
import axios from 'axios';

import { PopulateModelSourceDirectoryArray } from './PopulateModelSources';
import { PrefetchAssetUrls } from './PrefetchAssetUrls';

const args = process.argv.slice(2);

const shouldDownload = {
    models: args.includes('--models'),
    audio: args.includes('--audio'),
    sprites: args.includes('--sprites'),
};

if (!shouldDownload.models && !shouldDownload.audio && !shouldDownload.sprites) {
    shouldDownload.models = true;
    shouldDownload.audio = true;
    shouldDownload.sprites = true;
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
                        resolve(msg.completedCount);
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

        const results = await Promise.all(workerPromises);
        const totalDownloaded = results.reduce((acc: number, count: any) => acc + count, 0);

        console.log(
            `Finished downloading ${totalDownloaded} files for ${sourceType} into ${sources[0].outputDir}`
        );
    }

    return;
}

downloadAssets();
