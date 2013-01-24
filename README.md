Simple application for testing twitter access via openFrameworks, developed for the **El abismo no nos detiene** workshop (http://astrovandalistas.cc/abismo).

It basically builds a database of tweets based on different parameters and characteristics, and also sets up a thread to keep searching for tweets with specific hashtags in real-time. After building a database of tweets, a dynamic tag cloud is generated to help navigate the content and expose connections between different messages.

It currenlty needs the following libs that have to be downloaded separately:
- ofxOAuth (https://github.com/bakercp/ofxOAuth)
- ofxUI (https://github.com/rezaali/ofxUI)
- Jansson (https://github.com/akheron/jansson)

And these addons from a regular OF distro:
- ofxOsc
- ofxXmlSettings
