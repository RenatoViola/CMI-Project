var Module = {
  canvas: document.getElementById('canvas'),
  print: (...args) => console.log(...args),
  printErr: (...args) => console.error(...args),
  setStatus: (message) => {
    const status = document.getElementById('status');
    status.textContent = message;
    status.hidden = !message;
  },
  onRuntimeInitialized: () => {
    Module.setStatus('');
    Module.canvas.focus({ preventScroll: true });
  },
  onAbort: (reason) => Module.setStatus('The studio could not start. Please reload the page. Details: ' + reason)
};
document.getElementById('fullscreen').addEventListener('click', () => {
  if (Module.canvas.requestFullscreen) Module.canvas.requestFullscreen();
});
Module.canvas.addEventListener('webglcontextlost', (event) => {
  event.preventDefault();
  Module.setStatus('The graphics connection was lost. Reload the page to continue.');
});
if (!window.crossOriginIsolated) {
  Module.setStatus('Open this demo using the supplied local server or a host configured with the included headers.');
}
