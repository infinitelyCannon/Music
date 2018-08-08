import {Howl, Howler} from 'howler';

class Player{
    constructor(){
        this.queue = [];
        this.index = 0;
        this.isActive = false;
        this.root = document.getElementById('playbar');
        this.bar = {
            progress: this.root.querySelector('progress'),
            handle: this.root.querySelector('input')
        };

        this.root.querySelector('a:nth-child(4)').addEventListener('click', () =>{
            if(this.isActive){
                this.playPause();
            }
        });
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
                loop: true,
                onplay: () => {
                    this.root.querySelector('.title').innerHTML = track.title;
                    this.root.querySelector('#nowArtist').innerHTML = track.artist;
                    this.root.querySelector('img').src = track.cover;
                    this.root.querySelector('#time-end').innerHTML = track.duration.toTime();
                    this.root.querySelector('.mdi-36px').classList.remove("mdi-play");
                    this.root.querySelector('.mdi-36px').classList.add("mdi-pause");
                    requestAnimationFrame(this.step.bind(this));
                }
            });
        }

        sound.play();
        this.isActive = true;
        this.index = index;
    }

    playPause(){
        var sound = this.queue[this.index].howl;

        if(this.queue.length != 0){
            if(sound.playing()){
                sound.pause();
                this.root.querySelector('.mdi-36px').classList.remove("mdi-pause");
                this.root.querySelector('.mdi-36px').classList.add("mdi-play");
            }
            else{
                sound.play();
                this.root.querySelector('.mdi-36px').classList.remove("mdi-play");
                this.root.querySelector('.mdi-36px').classList.add("mdi-pause");
            }
        }
    }

    beginPlayback(list, start){
        if(this.isPlaying()){
            this.queue[this.index].howl.stop();
        }

        this.queue = list.map((item) =>{
            return Object.assign({}, item, {howl: null});
        });
        this.index = start;

        this.play(start);
    }

    step(){
        var sound = this.queue[this.index].howl;
        var seek = sound.seek() || 0;

        this.root.querySelector('#time-start').innerHTML = seek.toTime();
        this.bar.progress.value = this.bar.handle.value = (seek / sound.duration()) * 100;

        if(sound.playing()){
            requestAnimationFrame(this.step.bind(this));
        }
    }

    isPlaying(){
        if(this.queue.length === 0){
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
        return this.queue[this.index].howl;
    }

    setVolume(percent){
        let fraction = percent / 100;

        Howler.volume(fraction * fraction);
    }
}

export default Player;