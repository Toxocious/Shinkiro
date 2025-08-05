import * as cheerio from 'cheerio';
import axios from 'axios';

import { SourceDirectoryMap, getErrorMessage } from './Asset_Downloader';

export async function PrefetchAssetUrls(sourceDirectories: SourceDirectoryMap): Promise<string[]> {
    const promises = sourceDirectories.models.map(async (modelSource, i) => {
        try {
            const response = await axios.get(modelSource.url, {
                responseType: 'text',
                headers: {
                    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)...',
                    'Referer': 'https://www.models-resource.com/',
                },
            });

            const $ = cheerio.load(response.data);
            const zipHref = $('#content > div:nth-child(4) > a').attr('href');

            if (!zipHref) {
                console.warn(`No ZIP link found on ${modelSource.url}`);
                return null;
            }

            const fullZipUrl = new URL(zipHref, modelSource.url).href;

            return fullZipUrl;
        } catch (error) {
            console.error(`Error scraping ${modelSource.url}: ${getErrorMessage(error)}`);
            return null;
        }
    });

    const results = await Promise.all(promises);

    const fileUrls = results.filter((url): url is string => url !== null);

    if (fileUrls.length === 0) {
        console.warn('No ZIP URLs were found for models.');
    }

    return fileUrls;
}
