import Immutable from 'immutable';
import Song from './Song';
import Album from './Album';
import {ReduceStore} from 'flux/utils';
import MusicDispatcher from './MusicDispatcher';
import ActionTypes from './ActionTypes';
import _ from 'underscore';
import {ipcRenderer} from 'electron';
import hash from 'hash-sum';
import {URL} from 'url';

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
    if(data === undefined || data.length === 0){
        return 'dist/blankCover.png';
    }

    //fs.writeFileSync(path, data[0].data);
    /*, (err) => {
        if(err) throw err;
        webp = spawn('./lib/webp/cwebp.exe', [path, '-o', window.dataPath + '/img/' + hash(name) + '.webp']);
        webp.stdout.on('data', (data) => {
            
        });
        webp.stderr.on('data', (data) => {
            //console.log("Bin Error: " + data);
        });
        webp.on('close', (code) => {
            fs.unlink(path, (err) => {if(err){console.log("Deletion Error: " + err)}});
            //console.log("New Image!");
        });
    });
    */

    return window.dataPath + '/img/' + hash(name) + '.webp';
}

class SongStore extends ReduceStore{
    constructor(){
        super(MusicDispatcher);
    }

    getInitialState(){
        let config = window.eStore.get('state');
        let opening = {
            route: (config === undefined ? ["songs", "alphaDesc", "all"] : config.route),
            songs: Immutable.List(window.eStore.get('songs')),
            albums: null
        };

        return opening;
    }

    createSong(state, action){
        let songs = Immutable.List(window.eStore.get('songs'));
        let fileUrl = new URL('file://' + action.dir);
        let path = fileUrl.toString();

        if( songs.find((value) => {return value.id === path}) === undefined ){
            songs = songs.push(Object.assign({}, Song, {
                id: path,
                title: safeVal(action.info.common.title, decodeURI(path).slice(decodeURI(path).lastIndexOf('/') + 1, decodeURI(path).lastIndexOf('.'))),
                artist: safeVal(action.info.common.artists, "Unknown Artist"),
                albumArtist: safeVal(action.info.common.albumartist, safeVal(action.info.common.artists, "Unknown Artist")),
                album: safeVal(action.info.common.album, "Unknown Album"),
                year: safeVal(action.info.common.year, NaN),
                genre: safeVal(action.info.common.genre, ""),
                duration: action.info.format.duration,
                cover: safeImg(action.info.common.picture, path),
                coverType: safeVal(action.info.common.picture[0].format, ""),
                trackNum: safeVal(action.info.common.track.no, NaN),
                diskNum: safeVal(action.info.common.disk.no, NaN)
            }))
            .sort((a, b) => {
                if(a.title < b.title){return -1;}
                if(a.title > b.title){return 1;}
                if(a.title === b.title){return 0;}
            });

            ipcRenderer.sendSync('synchronous-message', {msg: 'addSong', data: songs.toArray()});
        }

        if(state.route[0] === 'songs'){
            return songs;
        }
        else{
            return null;
        }
    }

    createAlbum(state, action){
        let albums = Immutable.List(window.eStore.get('albums'));
        let tempIndex = albums.findIndex((value) => {return value.name === safeVal(action.info.common.album, "Unknown Album")});
        let tempUrl = new URL('file://' + action.dir);

        if(tempIndex === -1){
            albums = albums.push(Object.assign({}, Album, {
                name: safeVal(action.info.common.album, "Unknown Album"),
                year: safeVal(action.info.common.year, NaN),
                artist: safeVal(action.info.common.albumartist, safeVal(action.info.common.artists, "Unknown Artist")),
                genre: safeVal(action.info.common.genre, ""),
                cover: safeImg(action.info.common.picture, tempUrl.toString())/*(action.info.common.picture === undefined ? 'dist/blankCover.png' : window.dataPath + '/img/' + hash(tempUrl.toString()) + '.' + action.info.common.picture[0].format.slice(action.info.common.picture[0].format.indexOf('/') + 1))*/
            }));
            ipcRenderer.sendSync('synchronous-message', {msg: 'addAlbum', data: albums.toArray()});
        }
        else{
            if(action.info.common.trackNum === 1){
                albums = albums.update(tempIndex, (value) => Object.assign({}, value, {
                    name: safeVal(action.info.common.album, "Unknown Album"),
                    year: safeVal(action.info.common.year, NaN),
                    artist: safeVal(action.info.common.albumartist, safeVal(action.info.common.artists, "Unknown Artist")),
                    genre: safeVal(action.info.common.genre, ""),
                    cover: safeImg(action.info.common.picture, tempUrl.toString())/*(action.info.common.picture === undefined ? 'dist/blankCover.png' : window.dataPath + '/img/' + hash(tempUrl.toString()) + '.' + action.info.common.picture[0].format.slice(action.info.common.picture[0].format.indexOf('/') + 1))*/
                }));
                ipcRenderer.sendSync('synchronous-message', {msg: 'addAlbum', data: albums.toArray()});
            }
        }

        if(state.route[0].indexOf('Album') != -1){
            return albums;
        }
        else{
            return null;
        }
    }

    updateSongs(nextRoute){
        switch(nextRoute[0]){
            case 'songs':
                return Immutable.List(window.eStore.get('songs'));
            case 'oneAlbum':
                return Immutable.List(window.eStore.get('songs')).filter((val) => {val.album === nextRoute[2]});
            default:
                return null;
        }
    }

    updateAlbums(nextRoute){
        switch(nextRoute[0]){
            case 'allAlbums':
            case 'oneAlbum':
                return Immutable.List(window.eStore.get('albums'));
            default:
                return null;
        }
    }

    reduce(state, action){
        let result;
        switch(action.type){
            case ActionTypes.ADD_SONG:
                result = Object.assign({}, state, {
                    songs: this.createSong(state, action),
                    albums: this.createAlbum(state, action)
                });
                return result;
            case ActionTypes.NAVIGATE_UI:
                result = Object.assign({}, state, {
                    route: [action.primary, action.secondary, action.tertiary],
                    songs: this.updateSongs([action.primary, action.secondary, action.tertiary]),
                    albums: this.updateAlbums([action.primary, action.secondary, action.tertiary])
                });
                return result;
            default:
                return state;
        }
    }
}

export default new SongStore();