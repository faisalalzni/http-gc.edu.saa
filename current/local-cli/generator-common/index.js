const fs = require('fs');
const chalk = require('chalk');
const path = require('path');
const copyAndReplace = require('react-native-local-cli/util/copyAndReplace');
const walk = require('react-native-local-cli/util/walk');
const prompt = require('react-native-local-cli/generator/promptSync')();

function createDir(destPath) {
  if (!fs.existsSync(destPath)) {
    fs.mkdirSync(destPath);
  }
}

function copyAndReplaceWithChangedCallback(srcPath, destRoot, relativeDestPath, replacements) {
  if (!replacements) {
    replacements = {};
  }
  const contentChangedCallback = (_, contentChanged) =>
    upgradeFileContentChangedCallback(
      srcPath,
      relativeDestPath,
      contentChanged
    );

  copyAndReplace(
    srcPath,
    path.join(destRoot, relativeDestPath),
    replacements,
    contentChangedCallback
  );
}

function copyAndReplaceAll(srcPath, destPath, relativeDestDir, replacements) {
  walk(srcPath).forEach(absoluteSrcFilePath => {
    const filename = path.relative(srcPath, absoluteSrcFilePath);
    const relativeDestPath = path.join(relativeDestDir, filename);
    copyAndReplaceWithChangedCallback(absoluteSrcFilePath, destPath, relativeDestPath, replacements);
  });
}

function upgradeFileContentChangedCallback(
  absoluteSrcFilePath,
  relativeDestPath,
  contentChanged
) {
  if (contentChanged === 'new') {
    console.log(`${chalk.bold('new')} ${relativeDestPath}`);
    return 'overwrite';
  }
  if (contentChanged === 'changed') {
    console.log(
      `${chalk.bold(relativeDestPath)} ` +
      `has changed in the new version.\nDo you want to keep your ${relativeDestPath} or replace it with the ` +
      `latest version?\nIf you ever made any changes ` +
      `to this file, you'll probably want to keep it.\n` +
      `You can see the new version here: ${absoluteSrcFilePath}\n` +
      `Do you want to replace ${relativeDestPath}? ` +
      `Answer y to replace, n to keep your version: `
    );
    const answer = prompt();
    if (answer === 'y') {
      console.log(`Replacing ${relativeDestPath}`);
      return 'overwrite';
    }
    console.log(`Keeping your ${relativeDestPath}`);
    return 'keep';
  }
  if (contentChanged === 'identical') {
    return 'keep';
  }
  throw new Error(
    `Unknown file changed state: ${relativeDestPath}, ${contentChanged}`
  );
}

module.exports = {
  createDir, copyAndReplaceWithChangedCallback, copyAndReplaceAll
};
