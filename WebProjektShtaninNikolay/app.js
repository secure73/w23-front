//Crazy Nicky
import data from "dest/input.json" assert {type: json};
const fs = require('fs');
const { type } = require('os');
const { json } = require('stream/consumers');
var color = "#0B9880";
let okBtn = index.querySelector('#okBtn');
okBtn.addEventListener('click', function(){
    color = index.querySelector('#color');
    stylesheet.color = color;
});

let sendBtn = contat.querySelector('#sendBtn');
sendBtn.addEventListener('click', sendRecord);
function sendRecord(){
    let anrede = contact.querySelector('#anrede');
    let firstname = contact.querySelector('#firstname');
    let secondname = contact.querySelector('#secondname');
    let email = contact.querySelector('#email');
    let nachricht = contact.querySelector('#nachricht');
    let newRecord = new {color,anrede,firstname,secondname,email,nachricht};
    data.push(newRecord);
    fs.writeFileSync('dest/input.json', JSON.stringify(data.default));
}

