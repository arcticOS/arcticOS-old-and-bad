# arcticOS Services Documentation
Services in arcticOS are designed to run separate from the launcher and applications. They are meant to be used for things that always need to be running - like user input, display, and audio. They accept commands via an HTTP server. Third-party services may handle interaction however the developers see fit, but official services must follow these guidelines:

### HTTP Server Port
Every service must listen on its own port. Currently used ports are:
- `3502` - DisplayService
- `3503` - InputService
- `3504` - TimingService
- `3505` - WatchdogService

### API
API calls made to services are done by making a GET request in the following format:

```
  http://localhost:<service port>/<service id>/<command>
```

Commands are base64-encoded before being passed into the URL, and in their unencoded form are structured using the following format:

```
  <command id>\r<argument>\r<argument>\r<argument>\r
```

Each command may have its own set and number of arguments.

### Response
Any data passed back to the application (for example, querying which keys are pressed from InputService) must be returned using the HTTP response body.