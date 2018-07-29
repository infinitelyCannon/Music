import React from 'react';
import _ from 'lodash';

function sorting(values, parms){
    var stageOne = _.sortBy(values, (item) => {return item[parms.sort.type].toLowerCase()});

    if(parms.filter != "All"){
        stageOne = _.filter(stageOne, {genre: parms.filter});
    }
     
    return () => {
        if(!parms.sort.ascend){
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
                    <th>Artist</th>
                    <th>Album</th>
                    <th>
                        <span className="icon is-medium">
                            <i className="mdi mdi-24px mdi-clock-outline"></i>
                        </span>
                    </th>
                </tr>
            </thead>
            <tbody>
                {
                    _.map(sorting(music, route)(), (item) => (
                        <tr key={item.id}>
                            <td>{item.title}</td>
                            <td>{item.artist}</td>
                            <td>{item.album}</td>
                            <td>{item.duration.toTime()}</td>
                        </tr>
                    ))
                }
            </tbody>
        </table>
    );
}

export default Songs;