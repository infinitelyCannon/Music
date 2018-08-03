import React from 'react';
import _ from 'lodash';

function sorting(values, params){
    var stageOne = _.sortBy(values, (item) => {
        if(typeof item[params.sort.type] === 'string'){
            return item[params.sort.type].toLowerCase().replace(/^a\s+|^the\s+/gi, '');
        }
        else{
            return item[params.sort.type];
        }
    });

    if(params.filter != "All"){
        stageOne = _.filter(stageOne, {genre: params.filter});
    }
     
    return () => {
        if(!params.sort.ascend){
            return _.reverse(stageOne);
        }
        else{
            return stageOne;
        }
    }
}

const Songs = ({music, route}) => {
    return (
        <table className="table is-fullwidth is-striped is-hoverable">
            <thead>
                <tr>
                    <th>Name</th>
                    <th>
                        <span className="icon is-medium">
                            <i className="mdi mdi-24px mdi-clock-outline"></i>
                        </span>
                    </th>
                    <th>Artist</th>
                    <th>Album</th>
                </tr>
            </thead>
            <tbody>
                {
                    _.map(sorting(music, route)(), (item) => (
                        <tr key={item.id}>
                            <td><span style={{marginLeft: '20px'}} className="song-title">{item.title}</span></td>
                            <td>{item.duration.toTime()}</td>
                            <td>{item.artist}</td>
                            <td>{item.album}</td>
                        </tr>
                    ))
                }
            </tbody>
        </table>
    );
}

export default Songs;