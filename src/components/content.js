import React from 'react';
import Albums from './albums';
import Songs from './songs';

const Content = ({music, onNameClick, route}) => {
    var content;
    switch(route.view){
        case "albums":
            content = Albums;
            break;
        case "songs":
            content = Songs;
            break;
        default:
            content = () => <p>You shouldn't see this!!</p>
            break;
    }

    return (
        <div id="container" style={{paddingTop: '20px'}}>
            {
                content({music, onNameClick, route})
            }
        </div>
    );
};

export default Content;