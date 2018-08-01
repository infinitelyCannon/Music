import React from 'react';
import _ from 'lodash';
import Img from 'react-image';

function createList(data){
    let letter = '';
    var result = [];

    for(let i = 0; i < data.length; i++){
        if(letter != data[i].name[0]){
            letter = data[i].name[0];
            result.push({header: true, value: letter});
            result.push(data[i]);
        }
        else{
            result.push(data[i]);
        }
    }

    return result;
}

const Artists = ({music, onNameClick, route}) => {
    let list = createList(_.sortBy(music, (item) => {return item.name.toLowerCase().replace(/^a\s+|^the\s+/gi, '')}));
    var letterCount = 1;

    return (
        _.map(list, (item) =>(
            (item.header) ?
                <article key={item.value} className="message" style={{zIndex: letterCount++ }}>
                    <div className="message-header">
                        <h1 className="title">{item.value}</h1>
                    </div>
                </article>
                :
                <div key={item.name} className="card artist">
                    <div className="card-image">
                        <figure className="image is-1by1">
                            <Img src={[item.photo, 'dist/person.png']} />
                        </figure>
                    </div>
                    <div className="card-content">
                        <h4 className="title is-4">{item.name}</h4>
                    </div>
                </div>
        ))
    );
}

export default Artists;