# CrappyButtonBox
Arduino Button Box One

This is still very much a work in progress, but this is the Arduino sketch for a 26 button, 4 axis button box that I am building.

It probably won't work.

HOWEVER, if it does, this might be useful for someone.

I've elected (so far) to NOT use bitwise math to keep track of individual button states, but rather a couple of arrays. Is this less efficient?
Certainly.
Do I care?
Nah. Given that I'm including what will probably wind up being a 50ms bounce protection, and because this is a box of switches and buttons for a flight sim
and not a true gamepad-type controller that needs quick, twitchy inputs. I'm pretty sure I can accept whatever delay comes with this architecture.

Plus it just doesn't melt my brain.

EDIT:
In a few days I'll actually get to begin testing this code. I know that there are some hardware issues in the box,
mostly because I've never built dupont connectors before. Lots of troubleshooting coming up.

The code itself is maybe a workable scaffold that I can mess with over the coming week.

Could I look up someone else's code to make this work? Yeah! Am I going to? No!

Why?

I like the PAIN.

-JD
