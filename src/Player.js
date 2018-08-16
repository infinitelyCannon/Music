import {Howl, Howler} from 'howler';
import _ from 'lodash';

class Player{
    constructor(){
        this.queue = [];
        this.index = -1;
        this.isActive = false;
    }

    play(index){
        var sound;
        index = typeof index === 'number' ? index : this.index;
        var track = this.queue[index];

        if(track.howl){
            sound = track.howl;
        }
        else{
            sound = this.queue[index].howl = new Howl({
                src: [track.id],
                html5: true,
                onplay: () => {
                    this.onPlayTrigger();
                    this.isActive = true;
                },
                onend: () => {
                    this.onEndTrigger();
                }
            });
        }

        sound.play();
        this.index = index;
    }

    playPause(){
        if(this.queue.length === 0) return;
        var sound = this.queue[this.index].howl;

        if(sound.playing()){
            sound.pause();
        }
        else{
            sound.play();
        }
    }

    skip(dir){
        if(this.queue.length === 0) return;
        var index = 0;

        if(dir === 'prev'){
            index = this.index - 1;
            if(index < 0){
                index = this.queue.length - 1;
            }
        }
        else{
            index = this.index + 1;
            if(index >= this.queue.length){
                index = 0;
            }
        }

        this.skipTo(index);
    }

    skipTo(index){
        if(this.queue[this.index].howl){
            this.queue[this.index].howl.stop();
        }

        this.newIndex(index);
        this.newTrack(this.queue[index].id);
        this.play(index);
    }

    beginPlayback(list, start, rando){
        let tempID = list[start].id;
        if(this.isPlaying()){
            this.queue[this.index].howl.stop();
            this.cleanUp();
        }

        if(rando){
            this.queue = _.shuffle(_.map(list, (item) =>{
                return Object.assign({}, item, {howl: null});
            }));
            this.index = _.findIndex(this.queue, (item) => {return item.id === tempID});
        }
        else{
            this.queue = _.map(list, (item) =>{
                return Object.assign({}, item, {howl: null});
            });
            this.index = start;
        }

        this.skipTo(this.index);
    }

    scramble(){
        var tempID = this.queue[this.index].id;
        this.queue = _.shuffle(this.queue);
        this.index = _.findIndex(this.queue, (item) => {return item.id === tempID});
    }

    unscramble(){
        var tempID = this.queue[this.index].id;
        this.queue = _.sortBy(this.queue, (item) => {return item.trackNum.no});
        this.index = _.findIndex(this.queue, (item) => {return item.id === tempID});
    }

    stop(){
        this.isActive = false;
        if(this.isPlaying()){
            this.queue[this.index].howl.stop();
        }

        this.cleanUp();
    }

    seek(per){
        var sound = this.queue[this.index].howl;

        sound.seek(sound.duration * (per / 100));
    }

    cleanUp(){
        this.queue.forEach(function(val){
            if(val.howl != null){
                val.howl.unload();
            }
        });
    }

    setDispatcher(type, func){
        switch(type){
            case 'indx':
                this.newIndex = func;
                break;
            case 'id':
                this.newTrack = func;
                break;
            case 'end':
                this.onEndTrigger = func;
                break;
            default:
                break;
        }
    }

    setPlayTrigger(func){
        this.onPlayTrigger = func;
    }

    isPlaying(){
        if(this.queue.length === 0 || !this.isActive){
            return false;
        }
        else{
            return this.queue[this.index].howl.playing();
        }
    }

    getQueue(){
        return [...this.queue];
    }

    getNowPlaying(){
        return this.queue[this.index];
    }

    getProgress(){
        if(this.isActive){
            return this.queue[this.index].howl.seek();
        }
        else{
            return 0;
        }
    }

    getIndex(){
        return this.index;
    }

    setVolume(percent){
        let fraction = percent / 100;

        Howler.volume(fraction * fraction);
    }
}

export default Player;