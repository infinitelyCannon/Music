var mm = window.mm = require('music-metadata');
var Store = require('electron-store');
var remote = require('electron').remote;
var pic;
window.dialog = remote.dialog;
window.eStore = new Store();

require('electron').ipcRenderer.on('path', (event, message) => {
    window.dataPath = message;
});

//Note: Move this to React code to once audio plater is setup
document.addEventListener('drop', function(e){
    e.preventDefault();
    e.stopPropagation();

    for(let f of e.dataTransfer.files){
        read(f.path);
    }
});

document.addEventListener('dragover', function(e){
    e.preventDefault();
    e.stopPropagation();
});

function read(place){
    mm.parseFile(place, {native: true, mergeTagHeaders: true})
        .then(function(metadata){
            pic = metadata;
            console.log(metadata);
        })
        .catch(function(err){
            console.error(err.message);
    });
}
//End note.

Number.prototype.toTime = function(){
    var sec_num = this;
    var hours = Math.floor(sec_num / 3600);
    var minutes = Math.floor((sec_num - (hours * 3600) ) / 60);
    var seconds = Math.floor(sec_num - (hours * 3600) - (minutes * 60));
    
    if(hours < 10){hours = "0"+hours;}
    if(minutes < 10 && hours != "00"){minutes = "0"+minutes;}
    if(seconds < 10){seconds = "0"+seconds;}
    return ((hours == "00" ? '' : hours + ':') + minutes + ':' + seconds);
}