The way I have implemented BT is basically using a concept of LCV. Following can explain it better:


1. I start by pushing required L into the vector.


2. While inserting a value in the variable, I am not selecting it on random. I am maintaining a counter which starts from 0 and is incremented by 1 each time I push it in a new variable. This leaves enough room for other variables to choose values.

3. Using this technique there is no need to do Forward Checking as I am not creating any domains for any of the variables. Refer Point 2 above.


4. The same applies for Constraint Propagation.

If required, I can explain it better during the demo session.