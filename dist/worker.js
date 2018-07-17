const fs = require('fs');

onmessage = main;

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
                    postMessage(file);
                    break;
                default:
                    break;
            }
        }
    }
}

function main(e){
    for(var i = 0; i < e.data.length; i++){
        mainHelper(e.data[i]);
    }

    postMessage(0);
}