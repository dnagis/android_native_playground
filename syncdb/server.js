/*
 * 
 * Réception de POST data.
 * 
 * depuis le tarball de 2019:
 * 
 * 
 * 	NODE_PATH=/usr/lib64/node_modules/ node server.js #(mettre les nouveaux packages dans NODE_PATH):
	#nodemon --> cherche pas faut ***npm install -g nodemon*** à chaque fois bah oui c comme ça je vais pas y passer la nuit!!
	NODE_PATH=/usr/lib64/node_modules/ nodemon server.js
 * 
 * 
 * 
 * 
 * 
 */

'use strict';

const Hapi = require('hapi');

const server = Hapi.server({
    port: 8000,
    host: '0.0.0.0'
});

server.route({
    method: 'POST',
    path: '/',
    handler: (request, h) => {
		console.log('recu: time=' + request.payload.time + ' mac=' + request.payload.mac 
		+ ' temp=' + request.payload.temp);
        return 'OK';
    }
});

const init = async () => {

    await server.start();
    console.log(`Server running at: ${server.info.uri}`);
};

process.on('unhandledRejection', (err) => {

    console.log(err);
    process.exit(1);
});

init();
