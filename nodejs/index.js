import net from 'net';
import chalk from 'chalk';

console.clear();


const remoteHost='xtensa32plus.ddns.net';
const remotePort=230;



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

  if(commandsCounter<commands.length){
    console.log(`+<${chalk.magenta(commands[commandsCounter])}>`);
    client.write(`${commands[commandsCounter++]}\r\n`);
  }
    

});

client.on('close', ()=> {
  console.log(chalk.red('--DISCONNECTED--'));
});

grbl('g0');
grbl('x10');
grbl('g0');

