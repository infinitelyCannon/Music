const fs = require('fs');
const music = require('music-metadata');
const {URL} = require('url');
const hash = require('hash-sum');

onmessage = main;
var output = [];
var outLen = 0;
var fileNames = [];

function addMeta(item, doesExist){
    item.common.picture = doesExist;
    output.push(item);
    outLen++;

    if(outLen >= fileNames.length){
        postMessage(output);
    }
}

function mainHelper(dir){
    var items = fs.readdirSync(dir);
    var stat;

    for(var i = 0; i < items.length; i++){
        var file = dir + '/' + items[i];

        stat = fs.statSync(file);
        if(stat.isDirectory()){
            mainHelper(file);
        }
        if(stat.isFile()){
            switch( file.slice(file.lastIndexOf('.') + 1, file.length) ){
                case 'mp3':
                case 'wav':
                case 'flac':
                case 'acc':
                case 'ogg':
                case 'aif':
                case 'aiff':
                case 'aifc':
                case 'ape':
                case 'wma':
                case 'asf':
                case 'm4a':
                case 'oga':
                    fileNames.push(file);
                    break;
                default:
                    break;
            }
        }
    }
}

function main(e){
    for(let i = 0; i < e.data[0].length; i++){
        mainHelper(e.data[0][i]);
    }

    for(let j = 0; j < fileNames.length; j++){
        music.parseFile(fileNames[j])
        .then((metadata) => {
            var path = new URL('file://' + fileNames[j]);
            if(metadata.common.picture != undefined){
                /*fs.writeFileSync(
                    e.data[1] + 
                    '/img/' +
                    hash(path.toString()) +
                    '.' +
                    metadata.common.picture[0].format.slice(metadata.common.picture[0].format.indexOf('/') + 1), metadata.common.picture[0].data);*/
                addMeta(Object.assign({}, metadata, {
                    fileName: path.toString(),
                    name: fileNames[j].slice(fileNames[j].lastIndexOf('/') + 1, fileNames[j].lastIndexOf('.'))
                }), true);
            }
            else{
                addMeta(Object.assign({}, metadata, {
                    fileName: path.toString(),
                    name: fileNames[j].slice(fileNames[j].lastIndexOf('/') + 1, fileNames[j].lastIndexOf('.'))}), false);
            }
        })
    }

    //postMessage(0);
}