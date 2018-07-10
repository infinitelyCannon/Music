import React from 'react';
import {Container} from 'flux/utils';
import RootView from '../views/root';
import SongStore from '../data/SongStore';
import MusicActions from '../data/MusicActions';

class AppContainer extends React.Component{
    static getStores(){
        return [SongStore];
    }

    static calculateState(prevState){
        return {
            songs: SongStore.getState(),
            onAdd: MusicActions.addSong
        };
    }

    render(){
        return <RootView songs={this.state.songs} onAdd={this.state.onAdd} />;
    }
}

export default Container.create(AppContainer)