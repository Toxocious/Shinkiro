import axios from 'axios';
import * as cheerio from 'cheerio';
import * as fs from 'fs/promises';
import * as path from 'path';
import { Worker } from 'worker_threads';
import * as os from 'os';

async function downloadPokemonSprites() {
    // Define source directories and their corresponding output directories
    const sourceDirectories = [
        {
            url: 'https://play.pokemonshowdown.com/sprites/gen5/',
            outputDir: './Sprites/Normal',
        },
        {
            url: 'https://play.pokemonshowdown.com/sprites/gen5-back/',
            outputDir: './Sprites/Normal-Backs',
        },
        {
            url: 'https://play.pokemonshowdown.com/sprites/gen5-shiny/',
            outputDir: './Sprites/Shiny',
        },
        {
            url: 'https://play.pokemonshowdown.com/sprites/gen5-back-shiny/',
            outputDir: './Sprites/Shiny-Backs',
        },
        {
            url: 'https://play.pokemonshowdown.com/sprites/itemicons/',
            outputDir: './Sprites/Items',
        },
        {
            url: 'https://play.pokemonshowdown.com/sprites/categories/',
            outputDir: './Sprites/Categories',
        },
        {
            url: 'https://play.pokemonshowdown.com/sprites/types/',
            outputDir: './Sprites/Types',
        },
    ];

    // Use CPU count for determining thread count (leave one core free)
    const NUM_WORKERS = Math.max(1, os.cpus().length - 1);

    let totalSprites = 0;
    let totalDownloaded = 0;

    // Process each directory
    for (const { url, outputDir } of sourceDirectories) {
        try {
            // Create output directory if it doesn't exist
            await fs.mkdir(outputDir, { recursive: true });

            console.log(`\nFetching sprite directory: ${url}`);
            const response = await axios.get<string>(url);
            const $ = cheerio.load(response.data);

            // Extract all PNG links
            const spriteUrls: string[] = [];
            $('.dirlist li a.row').each((_i, element) => {
                const href = $(element).attr('href');
                if (href && href.endsWith('.png')) {
                    spriteUrls.push(href.replace('./', ''));
                }
            });

            console.log(
                `Found ${spriteUrls.length} sprites to download using ${NUM_WORKERS} workers`
            );
            totalSprites += spriteUrls.length;

            // Split URLs into chunks for each worker
            const chunkSize = Math.ceil(spriteUrls.length / NUM_WORKERS);
            const chunks: string[][] = [];

            for (let i = 0; i < spriteUrls.length; i += chunkSize) {
                chunks.push(spriteUrls.slice(i, i + chunkSize));
            }

            // Create and run workers
            const workers = chunks.map((chunk, index) => {
                return new Promise<number>((resolve, reject) => {
                    const worker = new Worker('./sprite_worker.js', {
                        workerData: {
                            spriteUrls: chunk,
                            baseUrl: url,
                            outputDir: outputDir,
                            workerId: index + 1,
                            sourceDir: path.basename(url),
                        },
                    });

                    worker.on('message', (message) => {
                        if (message.type === 'progress') {
                            process.stdout.write(
                                `${message.sourceDir} Worker ${message.workerId}: ${message.completed}/${message.total} sprites\r`
                            );
                        } else if (message.type === 'complete') {
                            console.log(
                                `\n${message.sourceDir} Worker ${message.workerId} finished: Downloaded ${message.completedCount} sprites`
                            );
                            totalDownloaded += message.completedCount;
                            resolve(message.completedCount);
                        }
                    });

                    worker.on('error', reject);
                    worker.on('exit', (code) => {
                        if (code !== 0) {
                            reject(new Error(`Worker ${index + 1} exited with code ${code}`));
                        }
                    });
                });
            });

            // Wait for all workers in this directory to complete
            const results = await Promise.all(workers);
            const dirDownloaded = results.reduce((sum, count) => sum + count, 0);
            console.log(
                `\nCompleted ${url}: ${dirDownloaded}/${spriteUrls.length} sprites saved in ${outputDir}`
            );
        } catch (error: unknown) {
            const errorMessage = error instanceof Error ? error.message : String(error);
            console.error(`Error processing directory ${url}:`, errorMessage);
            // Continue with next directory even if one fails
        }
    }

    console.log(`\nAll downloads complete! ${totalDownloaded}/${totalSprites} sprites saved`);
}

downloadPokemonSprites();
