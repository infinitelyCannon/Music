export function toTime(num){
    if(num === undefined || num === NaN || typeof num != 'number'){
        console.log(num);
        console.warn("Invalid input.");
        return "0";
    }
    var sec_num = num;
    var hours = Math.floor(sec_num / 3600);
    var minutes = Math.floor((sec_num - (hours * 3600) ) / 60);
    var seconds = Math.floor(sec_num - (hours * 3600) - (minutes * 60));
    
    if(hours < 10){hours = "0"+hours;}
    if(minutes < 10 && hours != "00"){minutes = "0"+minutes;}
    if(seconds < 10){seconds = "0"+seconds;}
    return ((hours == "00" ? '' : hours + ':') + minutes + ':' + seconds);
}