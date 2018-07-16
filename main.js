const {app, BrowserWindow, ipcMain} = require('electron');
const {default: installExtension, REACT_DEVELOPER_TOOLS} = require('electron-devtools-installer');
const Store = require('electron-store');
const fs = require('fs');
const eStore = new Store();

let win;

if(!fs.existsSync(app.getPath("userData") + "/img/")){
    fs.mkdir(app.getPath("userData") + "/img/", (err) => {if(err){console.error(err)}});
}

function createWindow(){
    win = new BrowserWindow({
        width: 900,
        height: 800,
        webPreferences: {nodeIntegrationInWorker: true},
        frame: false
    });

    win.loadFile('index.html');

    win.webContents.on('did-finish-load', () => {
        win.webContents.send('path', app.getPath('userData'));
    });

    installExtension(REACT_DEVELOPER_TOOLS)
        .then((name) => console.log('Added Extension: ' + name))
        .catch((err) => console.log('An error occurred: ', err));

    win.webContents.openDevTools();

    win.on('maximize', () =>{
        win.webContents.send('maxed', true);
    });

    win.on('unmaximize', () => {
        win.webContents.send('maxed', false);
    });

    win.on('closed', () => {
        win = null;
    });
}

app.on('ready', createWindow);

ipcMain.on('asynchronous-message', (event, arg) => {
    if(arg === 'exit'){
        win.close();
    }
    if(arg === 'mini'){
        win.minimize();
    }
    else if(arg.msg === 'addSong'){
        eStore.set('songs', arg.data);
        event.sender.send('asynchronous-reply', "Done.");
    }
    if(arg.msg === 'addAlbum'){
        eStore.set('albums', arg.data);
    }
});

ipcMain.on('synchronous-message', (event, arg) =>{
    if(arg === 'max'){
        if(win.isMaximized()){
            win.unmaximize();
            event.returnValue = false;
        }
        else{
            win.maximize();
            event.returnValue = true;
        }
    }
});

app.on('window-all-closed', () => {
    if(process.platform !== 'darwin'){
        app.quit();
    }
});

app.on('activate', () => {
    if(win === null){
        createWindow();
    }
});