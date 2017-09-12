GagBook
=======

GagBook is an unofficial mobile app for [9GAG](http://9gag.com) website, currently available
for SailfishOS, MeeGo Harmattan and Symbian^3.

This is a fork of the original GagBook by dicksonleong mainly focussing on updateting the UI for SailfishOS
and adding some features as the GagBook wasn't updated in years.

GagBook get data from 9GAG by scraping their website, or using [InfiniGAG](https://github.com/k3min/infinigag)
API. This might be unethical but I made this app because there is no official 9GAG app for MeeGo Harmattan
and Symbian^3.

Features
--------
- Simple, smooth and native UI
- View posts in different sections, eg. hot, trending, fresh, etc
- Full screen view of image with pinch-to-zoom
- Login (with email only) and voting
- View comments for each post
- Save image
- Share post using Harmattan's integrated sharing
- Video support (SailfishOS only atm. support for MP4 Files on 9GAG)

Limitation / Known Issues / To Do
---------------------------------
- **GagBook may fail to work anytime when 9GAG update their website** (if this happens, you will see "error: Unable to parse response")
- Image upload is not supported
- Large image will be stretched down due to hardware limitation

Contact me
----------

- Email: leszek.lesner@web.de

Contact the Original Author
---------------------------
- Twitter: [@DicksonBeta](http://twitter.com/DicksonBeta)
- Email: dicksonleong8@gmail.com

License
-------

GagBook is licensed under the Simplified BSD License, see the file "LICENSE" for more info.

GagBook use the [Qt-Json library](https://github.com/ereilin/qt-json), which is also licensed
under the Simplified BSD License, see the file "qt-json/LICENSE" for more info.

VideoComponent.qml is licensed under GPLv3
