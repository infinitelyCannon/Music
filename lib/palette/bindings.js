var palette;

if(process.env.DEBUG){
    palette = require('./build/Debug/palette.node');
}
else{
    palette = require('./build/Release/palette.node');
}

module.exports = palette;