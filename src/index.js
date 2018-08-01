import React from 'react';
import ReactDOM from 'react-dom';
//import thunkMiddleware from 'redux-thunk';

import {Provider} from 'react-redux';
import {createStore/*, applyMiddleware*/} from 'redux';
import rootReducer from './reducers';
import App from './components/app';
const {ipcRenderer} = require('electron');



//import SongStore from './data/SongStore';
/*
Note: After you add the change to make this a window without the top default UI bar,
check to make sure that Ctrl+Shift+I doesn't activate the devTools
*/
const store = createStore(rootReducer);

function persist(data){
    if(data.route.view.type === undefined){
        document.getElementById('pageBody').className = "has-navbar-fixed-bottom has-navbar-fixed-top is-tabbed";
    }
    else{
        document.getElementById('pageBody').className = "has-navbar-fixed-bottom has-navbar-fixed-top";
    }
    ipcRenderer.send('asynchronous-message', data);
}

function observeStore(store, onChange){
    let currentState;

    function handleChange(){
        let nextState = store.getState();
        if(nextState !== currentState){
            currentState = nextState;
            onChange(currentState);
        }
    }

    let unsubscribe = store.subscribe(handleChange);
    handleChange();
    return unsubscribe;
}

const unlinkStore = observeStore(store, persist);

ReactDOM.render(
<Provider store={store}>
    <App />
</Provider>, document.getElementById('main'));