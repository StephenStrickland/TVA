TVA
===
as soon as the video loads we get several parts of data: total frames and frame rate.

We set the first frame which then allows the user to draw a polygon on the screen.

When the user clicks analyze the tracker does not analyze the first 100 frames(3  secs) instead it models and evolves the background to the current environment.

after the tracker finishes the background modeling it starts to track. 

for each frame, this is the process:

process each frame through the motion detector,  various image processing and background subtraction.

pass the processed frame into the blob detector to find cars.

the blob detector returns a list of rectangles that contain the various blobs.

we then validate each rectangle making sure that it touches or is inside the polygon.

we pull a list of all cars that are not finished and compare these cars last know location with the list of new rectangles. If a rectangle matches we update the current rect and increment the elapsed frames. If a rectangle is not assigned to a car then it is added to the NewCars list.

The tracker then checks all cars that are not done, to see if they are still contained within the polygon, if not then it marks the car as done. 

Then we add the list of new cars to our master list of cars.

if our current frame count is modulus of 300 or equal to total frames we write the current car master list to a csv file on the user's desktop under a folder "Tracking Video". This is redundancy in case the application ever crashes.


I got rid of the ListView in order to have greater performance on the app. If you want to see the real time data watch the console. it will show you how many rects were passed into the tracker, how many were valid, and when new cars are created.


#####MPT is the application/project that was builtbefore we were added to the project.

