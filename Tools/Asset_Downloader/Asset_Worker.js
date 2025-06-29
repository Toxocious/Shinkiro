const { parentPort, workerData } = require('worker_threads');
const axios = require('axios');
const fs = require('fs/promises');
const path = require('path');

async function downloadSprites() {
    const { spriteUrls, baseUrl, outputDir, workerId, sourceDir } = workerData;
    let completed = 0;
    const total = spriteUrls.length;

    try {
        for (const spriteUrl of spriteUrls) {
            const fullUrl = baseUrl + spriteUrl;
            const outputPath = path.join(outputDir, spriteUrl);

            try {
                const imageResponse = await axios.get(fullUrl, {
                    responseType: 'arraybuffer',
                });
                await fs.writeFile(outputPath, Buffer.from(imageResponse.data));

                completed++;

                // Report progress every few downloads
                if (completed % 5 === 0 || completed === total) {
                    parentPort.postMessage({
                        type: 'progress',
                        workerId,
                        completed,
                        total,
                        sourceDir,
                    });
                }

                // Small delay to be nice to the server
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
