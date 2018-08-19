import React from 'react';
import Albums from './albums';
import Songs from './songs';
import Artists from './artists';
import SingleView from '../containers/SingleView';

const Content = ({music, onNameClick, route}) => {
    var content;
    switch(route.view){
        case "albums":
            content = Albums;
            break;
        case "songs":
            content = Songs;
            break;
        case "artists":
            content = Artists;
            break;
        default:
            content = () => <p>You shouldn't see this!!</p>
            break;
    }

    if(typeof route.view === "string"){
        return (
            <div id="container" style={{paddingTop: (route.view == 'artists' ? '0px' : '20px')}}>
                {
                    content({music, onNameClick, route})
                }
            </div>
        );
    }
    else{
        return <SingleView />;
    }
};

export default Content;