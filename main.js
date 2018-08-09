const {app, BrowserWindow, ipcMain, globalShortcut, Menu} = require('electron');
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
        webPreferences: {nodeIntegrationInWorker: true}
    });

    win.loadFile('index.html');

    win.webContents.on('did-finish-load', () => {
        win.webContents.send('path', app.getPath('userData'));
    });

    win.on('app-command', (e, cmd) => {
        switch(cmd){
            case 'browser-backward':
                win.webContents.send('navigate', cmd);
                break;
            case 'browser-forward':
                win.webContents.send('navigate', cmd);
                break;
            default:
                break;
        }
    });

    installExtension(REACT_DEVELOPER_TOOLS)
        .then((name) => console.log('Added Extension: ' + name))
        .catch((err) => console.log('An error occurred: ', err));

    win.webContents.openDevTools();

    win.on('closed', () => {
        win = null;
    });
}

app.on('ready', () => {
    /*For later
    const play = globalShortcut.register('MediaPlayPause', () => {
        console.log("PlayPaused Triggered.");
    });

    const skip = globalShortcut.register('MediaNextTrack', () => {
        console.log("MediaSkip Triggered.");
    });

    const prev = globalShortcut.register('MediaPreviousTrack', () => {
        console.log("MediaPrev Triggered.");
    });

    if(!play || !skip || !prev){
        console.log("Registration failed.");
    }

    console.log(globalShortcut.isRegistered('MediaPlayPause'));
*/
    //Menu.setApplicationMenu(null);
    createWindow();
});

app.on('window-all-closed', () => {
    if(process.platform !== 'darwin'){
        app.quit();
    }
});

app.on('will-quit', () => {
    globalShortcut.unregisterAll();
});

ipcMain.on('asynchronous-message', (event, arg) => {
    if(arg === 'exit'){
        win.close();
    }
    if(arg === 'mini'){
        win.minimize();
    }
    else{
        eStore.set('songs', arg.songs);
        eStore.set('albums', arg.albums);
        eStore.set('genres', arg.genres);
        eStore.set('route', arg.route);
        eStore.set('artists', arg.artists);
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
    if(arg.msg === 'addSong'){
        eStore.set('songs', arg.data);
        event.returnValue = null;
    }
    if(arg.msg === 'addAlbum'){
        eStore.set('albums', arg.data);
        event.returnValue = null;
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