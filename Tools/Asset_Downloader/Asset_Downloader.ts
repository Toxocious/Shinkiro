import * as cheerio from 'cheerio';
import * as fs from 'fs/promises';
import * as path from 'path';
import { Worker } from 'worker_threads';
import * as os from 'os';
import axios from 'axios';

async function downloadAssets() {
    // Define source directories and their corresponding output directories
    const sourceDirectories = {
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

    // Use CPU count for determining thread count (leave one core free)
    const NUM_WORKERS = Math.max(1, os.cpus().length - 1);

    const allSources = Object.values(sourceDirectories).flat();

    for (const source of allSources) {
        console.log(`\nStarting download for: ${source.outputDir}`);
        try {
            await fs.mkdir(source.outputDir, { recursive: true });

            const response = await axios.get(source.url);
            const $ = cheerio.load(response.data);
            const fileUrls = $('a')
                .map((i, link) => $(link).attr('href'))
                .get()
                .filter((href) => href.endsWith(source.fileType));

            if (fileUrls.length === 0) {
                console.log(`No files of type ${source.fileType} found at ${source.url}`);
                continue;
            }

            const filesPerWorker = Math.ceil(fileUrls.length / NUM_WORKERS);
            const workerPromises = [];
            let workersCompleted = 0;

            console.log(`Distributing ${fileUrls.length} files among ${NUM_WORKERS} workers...`);

            for (let i = 0; i < NUM_WORKERS; i++) {
                const start = i * filesPerWorker;
                const end = start + filesPerWorker;
                const workerFiles = fileUrls.slice(start, end);

                if (workerFiles.length === 0) continue;

                const workerPromise = new Promise((resolve, reject) => {
                    const worker = new Worker(path.resolve(__dirname, 'Asset_Worker.js'), {
                        workerData: {
                            spriteUrls: workerFiles, // worker script uses 'spriteUrls'
                            baseUrl: source.url,
                            outputDir: source.outputDir,
                            workerId: i + 1,
                            sourceDir: source.outputDir,
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
                                process.stdout.write('\n'); // New line after all progress updates for this source
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
                `Finished downloading for ${source.outputDir}. Total files: ${totalDownloaded}`
            );
        } catch (error) {
            console.error(`Failed to process source ${source.url}:`, error);
        }
    }
    console.log('\nAll asset downloads complete.');
}

downloadAssets();
