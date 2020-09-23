// Make connection
var socket = io.connect('http://localhost:3000');
var randomColor = Math.floor(Math.random()*16777215).toString(16);

// Query random
var btn = document.getElementById('burst');

(function(){
      const randomColor = Math.floor(Math.random()*16777215).toString(16);
      document.getElementsByClassName("fullwidth")[0].style.backgroundColor = "#" + randomColor;
      document.getElementsByClassName("fullwidth")[1].style.backgroundColor = "#" + randomColor;
})();

// Emit events
btn.addEventListener('click', function(){
      console.log('clicked');
      socket.emit('burst', {
            source: 1,
            destination: 2
      });
});
console.log('burst');

// Listen for events
socket.on('event', function(data){
});
