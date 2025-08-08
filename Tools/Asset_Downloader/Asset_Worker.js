const { parentPort, workerData } = require('worker_threads');
const axios = require('axios');
const fs = require('fs/promises');
const path = require('path');

async function downloadSprites() {
    const downloadedFiles = [];

    const { spriteUrls, baseUrl, outputDir, workerId, sourceDir } = workerData;
    let completed = 0;
    const total = spriteUrls.length;

    try {
        for (const spriteUrl of spriteUrls) {
            try {
                const url = new URL(spriteUrl); // full URL
                const fileName = path.basename(url.pathname); // strips query string
                const outputPath = path.join(outputDir, fileName);
                downloadedFiles.push(outputPath);

                const response = await axios.get(spriteUrl, {
                    responseType: 'arraybuffer',
                    headers: workerData.headers,
                });

                await fs.writeFile(outputPath, Buffer.from(response.data));

                completed++;
                if (completed % 5 === 0 || completed === total) {
                    parentPort.postMessage({
                        type: 'progress',
                        workerId,
                        completed,
                        total,
                        sourceDir,
                    });
                }

                await new Promise((resolve) => setTimeout(resolve, 50));
            } catch (error) {
                console.error(
                    `Worker ${workerId}: Failed to download ${spriteUrl}: ${error.message}`
                );
            }
        }

        // Report completion
        parentPort.postMessage({
            type: 'complete',
            workerId,
            completedCount: completed,
            sourceDir,
            downloadedFiles: downloadedFiles,
        });
    } catch (error) {
        console.error(`Worker ${workerId} error: ${error.message}`);
        parentPort.postMessage({
            type: 'error',
            workerId,
            error: error.message,
            sourceDir,
        });
    }
}

downloadSprites();
