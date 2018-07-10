import React from 'react';
import ReactDOM from 'react-dom';
import AppContainer from './containers/AppContainer';
//import SongStore from './data/SongStore';
/*
Note: After you add the change to make this a window without the top default UI bar,
check to make sure that Ctrl+Shift+I doesn't activate the devTools
*/

ReactDOM.render(<AppContainer />, document.getElementById('main'));