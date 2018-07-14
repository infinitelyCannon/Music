import Immutable from 'immutable';
import Song from './Song';
import {ReduceStore} from 'flux/utils';
import MusicDispatcher from './MusicDispatcher';
import ActionTypes from './ActionTypes';
import _ from 'underscore';
import {ipcRenderer} from 'electron';
import {spawn} from 'child_process';
import fs from 'fs';
import hash from 'hash-sum';

function safeVal(value, fallback){
    if(typeof value == undefined || value == null){
        return fallback;
    }
    if(typeof value == 'object' && value.length != undefined){
        return _.uniq(value).toString().replace(',', ', ');
    }
    else{
        return value;
    }
}

function safeImg(data, name){
    let webp, path;

    if(data === undefined || data.length === 0){
        return 'dist/blankCover.png';
    }

    path = window.dataPath + '/img/' + hash(name) + (data[0].format.indexOf('png') != -1 ? '.png' : '.jpeg');

    fs.writeFile(path, data[0].data, (err) => {
        if(err) throw err;
        webp = spawn('./lib/webp/cwebp.exe', [path, '-o', window.dataPath + '/img/' + hash(name) + '.webp']);
        webp.stdout.on('data', (data) => {
            
        });
        webp.stderr.on('data', (data) => {
            //console.log("Bin Error: " + data);
        });
        webp.on('close', (code) => {
            fs.unlink(path, (err) => {if(err){console.log("Deletion Error: " + err)}});
            console.log("New Image!");
        });
    });

    return window.dataPath + '/img/' + hash(name) + '.webp';
}

class SongStore extends ReduceStore{
    constructor(){
        super(MusicDispatcher);
    }

    getInitialState(){
        let music = window.eStore.get('songs');
        /* let albums = window.eStore.get('albums');
        let artists = window.eStore.get('artists'); */

        return ((music == undefined) ? Immutable.List() : Immutable.List(music));
    }

    reduce(state, action){
        let result, fileName = action.dir.replace('/', '\\');
        ipcRenderer.on('asynchronous-reply', (event, arg) => {
            //console.log(arg);
        });
        switch(action.type){
            case ActionTypes.ADD_SONG:
                if(state.find( (value) => {return value.id === action.dir.replace('/', '\\')}) === undefined){
                    result = state.push(Object.assign({}, Song, {
                        id: action.dir.replace('/', '\\'),
                        title: safeVal(action.info.common.title, fileName.slice(fileName.lastIndexOf('\\') + 1, fileName.lastIndexOf('.'))),
                        artist: safeVal(action.info.common.artists, "Unknown Artist"),
                        albumArtist: safeVal(action.info.common.albumartist, safeVal(action.info.common.artists, "Unknown Artist")),
                        album: safeVal(action.info.common.album, "Unknown Album"),
                        year: safeVal(action.info.common.year, NaN),
                        genre: safeVal(action.info.common.genre, ""),
                        duration: action.info.format.duration,
                        cover: safeImg(action.info.common.picture, action.dir.replace('/', '\\')),
                        coverType: safeVal(action.info.common.picture[0].format, ""),
                        trackNum: safeVal(action.info.common.track.no, NaN),
                        diskNum: safeVal(action.info.common.disk.no, NaN)
                    }))
                    .sort((a, b) =>{
                        if(a.title < b.title){return -1;}
                        if(a.title > b.title){return 1;}
                        if(a.title === b.title){return 0;}
                    });
                    ipcRenderer.send('asynchronous-message', result.toArray());
                    return result;
                }
                else{
                    return state
                }
            default:
                return state;
        }
    }
}

export default new SongStore();