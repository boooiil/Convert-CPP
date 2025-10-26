const { readdirSync, statSync, writeFileSync, existsSync } = require('fs');
const { extname } = require('path');

/**
 * 
 * @param {string} directory 
 */
function readDirectoryRecursive(directory) {

    if (!existsSync(directory)) throw new Error(`Directory does not exist: ${directory}`);

    const root = directory
    const files = readdirSync(root);

    let built = `# root/${root}meson.build\n\n`

    for (let file of files) {

        const path = root + file;
        const _stat = statSync(path);

        if (_stat.isDirectory()) {
            built += `subdir('${file}')\n`;
            readDirectoryRecursive(path + '/');
            continue;
        }

        if (extname(path).toLowerCase() !== '.cpp') continue;

        if (file.toLowerCase().includes('main.cpp')) {
            built += `src_files = files('${file}')\n`
        }
        else built += `src_files += files('${file}')\n`

    }

    // check if meson.build already exists
    if (existsSync(root + 'meson.build') && statSync(root + 'meson.build').isFile()) {
        var existing = require('fs').readFileSync(root + 'meson.build', 'utf-8');
        if (existing === built) {
            console.log(`${root}meson.build... Skipped (up to date).`);
            return;
        }
        else {
            console.log(`${root}meson.build... Updated.`);
        }
    }
    else {
        console.log(`${root}meson.build... Created.`);
    }

    writeFileSync(`${root}meson.build`, built);
}

readDirectoryRecursive('src/')
console.log('OK.');