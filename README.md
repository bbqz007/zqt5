#### a fork from zhelper_wxWidgets,  which can apply to qt5 window gui programming.

[github.com/bbqz007/zhelper-wxWidgets](github.com/bbqz007/zhelper-wxWidgets).

## how to use

* 1. run moc zqt_helper.h > zqt_helper_moc.h

* 2. include zqt_helper.h on your project's source code.

* 3. switch on c++14.

## how to write layouts?

* 1. begin with calling **layout::begin**(somelayout)

* 2. add elements(layout or widget) to layout, by calling **operator ()** (widget)

* 3. configure your latest added elements, by calling **operator []** (prop)

* 4. end with calling operator () **(layout::end)**

### just like 
```c++
layout::begin(somelayout)   // begin to write a layout
    [prop]              // configure layout properties of somelayout
    (new element)           // add a element to somelayout
        [prop]              // configure layout properties of new element
        [onclick = ...]     // configure onclick function associated with new element
        [onchar = ...]      // configure onchar function associated with new element
    (layout::end)           // end up writing a layout
```
