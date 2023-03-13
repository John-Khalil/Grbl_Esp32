import net from 'net';
import chalk from 'chalk';

console.clear();


const remoteHost='xtensa32plus.ddns.net';
const remotePort=230;
const commandDelay=10;



const commands=[];
let commandsCounter=0;

const grbl=command=>{
    commands.push(command);
}

const client = new net.Socket();

client.connect(remotePort, remoteHost, ()=> {
  console.log('CONNECTED');

//   client.write('$X\r\n');
});

client.on('data', (data)=> {
    console.log(`-<${chalk.cyan(data.toString().split('\r\n')[0])}>\r\n`);

    setTimeout(() => {
      if(commandsCounter<commands.length){
        console.log(`+<${chalk.magenta(commands[commandsCounter])}>`);
        client.write(`${commands[commandsCounter++]}\r\n`);
      }
    }, commandDelay);
});

client.on('close', ()=> {
  console.log(chalk.red('--DISCONNECTED--'));
});

grbl('$X');
// grbl('g0');
// grbl('x10');
// grbl('y50');

const parts=[];

const moveParts=()=>{
  grbl('$H');
  grbl('C0');
  parts.forEach(part => {
    grbl(`S${part.pumpSpeed}`);
    grbl(`G1 X${part.feeder.x} Y${part.feeder.y} F${part.feeder.feedRate}`);
    grbl(`G1 Z${part.feeder.z} F${part.feeder.feedRate}`);
    grbl(`M3`);
    grbl(`G1 Z0 F${part.pcb.feedRate}`);
    grbl(`G1 X${part.pcb.x} Y${part.pcb.y} F${part.pcb.feedRate}`);
    grbl(`G1 C${part.pcb.c} F${part.pcb.feedRate}`);
    grbl(`G1 Z${part.pcb.z} F${part.pcb.feedRate}`);
    grbl(`M5`);
    grbl(`G1 Z0 F${part.feeder.feedRate}`);
    grbl(`C0`);
  });
  grbl('$H');
}

