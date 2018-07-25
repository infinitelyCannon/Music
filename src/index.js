import React from 'react';
import ReactDOM from 'react-dom';
import thunkMiddleware from 'redux-thunk';
//import AppContainer from './containers/AppContainer';

import {Provider} from 'react-redux';
import {createStore, applyMiddleware} from 'redux';
import rootReducer from './reducers';
import App from './components/app';

//import SongStore from './data/SongStore';
/*
Note: After you add the change to make this a window without the top default UI bar,
check to make sure that Ctrl+Shift+I doesn't activate the devTools
*/
const store = createStore(rootReducer, applyMiddleware(thunkMiddleware));

ReactDOM.render(
<Provider store={store}>
    <App />
</Provider>, document.getElementById('main'));