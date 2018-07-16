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
            music: SongStore.getState(),
            onAdd: MusicActions.addSong,
            navigateUI: MusicActions.navigateUI
        };
    }

    render(){
        return <RootView music={this.state.music} onAdd={this.state.onAdd} navigate={this.state.navigateUI} />;
    }
}

export default Container.create(AppContainer)