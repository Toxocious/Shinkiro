import * as fs from 'fs';
import * as path from 'path';
import * as process from 'process';

import { spawn } from 'child_process';
import { promises as fsp } from 'fs';

/**
 * Change this to your local FBX Converter path.
 */
const FBX_CONVERTER_PATH =
    '"C:\\Program Files\\Autodesk\\FBX\\FBX Converter\\2013.3\\bin\\FBXConverter.exe"';

/**
 * Run a command using spawn with real-time stdout/stderr output.
 */
function runCommand(command: string, args: string[]): Promise<void> {
    return new Promise((resolve, reject) => {
        const proc = spawn(command, args, { shell: true });

        proc.stdout.on('data', (data) => process.stdout.write(data));
        proc.stderr.on('data', (data) => process.stderr.write(data));

        proc.on('close', (code) => {
            if (code === 0) resolve();
            else reject(new Error(`Process exited with code ${code}`));
        });
    });
}

/**
 * Converts all .fbx files in the inputDir, saving temp converted files in outputDir,
 * then backs up originals and overwrites originals with converted files.
 */
async function convertFbxFilesInDir(inputDir: string, outputDir: string) {
    const fbxFiles = fs.readdirSync(inputDir).filter((file) => file.toLowerCase().endsWith('.fbx'));

    if (!fs.existsSync(outputDir)) {
        fs.mkdirSync(outputDir, { recursive: true });
    }

    for (const fbxFile of fbxFiles) {
        const inputPath = path.join(inputDir, fbxFile);
        const baseName = path.basename(fbxFile, path.extname(fbxFile));
        const tempOutputPath = path.join(outputDir, `${baseName}.fbx`); // temporary conversion output

        try {
            console.log(`\nConverting: ${inputPath} -> ${tempOutputPath}`);
            await runCommand(FBX_CONVERTER_PATH, [inputPath, tempOutputPath]);
            console.log(`✅ Converted: ${tempOutputPath}`);

            // Backup original file
            const backupPath = `${inputPath}.bak`;
            await fsp.copyFile(inputPath, backupPath);
            console.log(`🗂️  Backed up original: ${backupPath}`);

            // Overwrite original file with converted file
            await fsp.copyFile(tempOutputPath, inputPath);
            console.log(`🔄 Replaced original with converted file: ${inputPath}`);

            // Optional: delete temp converted file
            // await fsp.unlink(tempOutputPath);
        } catch (err) {
            console.error(`❌ Failed to convert ${inputPath}:`, err);
        }
    }
}

/**
 * Convert .fbx files from ./Input/<folder>/ to ./Output/<folder>/ for every immediate subdirectory.
 */
async function convertAllSubdirectories(inputRoot: string, outputRoot: string) {
    const subdirs = fs
        .readdirSync(inputRoot, { withFileTypes: true })
        .filter((entry) => entry.isDirectory())
        .map((entry) => entry.name);

    for (const subdir of subdirs) {
        const inputDir = path.join(inputRoot, subdir);
        const outputDir = path.join(outputRoot, subdir);

        console.log(`\n=== Processing folder: ${subdir} ===`);
        await convertFbxFilesInDir(inputDir, outputDir);
    }
}

/**
 * Remove directory recursively
 */
async function removeDirectory(dirPath: string) {
    if (fs.existsSync(dirPath)) {
        await fsp.rm(dirPath, { recursive: true, force: true });
        console.log(`🗑️  Deleted directory: ${dirPath}`);
    }
}

/**
 * Entry point
 */
async function main() {
    const inputRoot = path.resolve('./Input');
    const outputRoot = path.resolve('./Output');

    if (!fs.existsSync(inputRoot)) {
        console.error(`❌ Input directory "${inputRoot}" does not exist.`);
        process.exit(1);
    }

    await convertAllSubdirectories(inputRoot, outputRoot);

    await removeDirectory(outputRoot);
}

main();
