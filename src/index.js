#!/usr/bin/env node
"use strict";

const React = require("react");
const importJsx = require("import-jsx");
const { render } = require("ink");
const meow = require("meow");
require("dotenv").config();

const { checkForEnvFile, checkWallet, checkArbReady } = require("./utils");

checkForEnvFile();
checkWallet();

const isArbReady = async () => {
  try {
    await checkArbReady();
    return true;
  } catch (error) {
    const chalk = require("chalk");
    const { logExit } = require("./exit");
    const spinner = { text: "" };
    spinner.text = chalk.black.bgRedBright(`\n${error.message}\n`);
    logExit(1, error);
    process.exit(1);
  }
};

isArbReady().then((arbReady) => {
  if (!arbReady) {
    process.exit(1);
  }
});

const wizard = importJsx("./wizard/index");

const cli = meow(`
  Usage
    $ solana-jupiter-bot

  Options
    --name  Your name

  Examples
    $ solana-jupiter-bot --name=Jane
    Hello, Master
`);

console.clear();
render(React.createElement(wizard, cli.flags)).waitUntilExit();
