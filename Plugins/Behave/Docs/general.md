| Dimensions 	| Has		| Does		| Wants		| Needs 	|
------------------------------------------
Space           | Position	 Move		 Go To
				  Rotation	 Open?		 Get Item
				  Scale


Emotional		 Anger/Fear/etc

Biological		 Energy/Hunger/Temp

Time			 ???

See this one
https://www.youtube.com/watch?v=gm7K68663rA#
it's loosely based on the first person.
And the comment about Layered GOAP

also look at state tree
it can do very similar stuff. and it's much cooler.
https://www.youtube.com/watch?v=YEmq4kcblj4
https://www.youtube.com/watch?v=zovPQnq7ndE

# TODO challenges/later
* Modifier curves
* Modifier in general (like the loop)
* * has to be a list so i can add them.
* * can be a struct
* A* plan search
* Randomize costs

# note
currently i have the actions do the actual logic and controlling the pawn.
a more general way would be to have the pawn listen to the current action and act accordingly. stopping the action if necessary. but i think that adds complexity and can make things more difficult.
you can do it that way if you prefer.

# TODO

* preferences/traits
* modifies the values of certain wants.
* could also modify values of the "damps" (at least on Do)
* traits could be 
* * personal
* * environmental (added/removed depending on the place)
* * some behaves can add/remove traits (e.g. sadness adds "slow" and "tiresome")
* * traits can also be queried to inform the representation (visual/audio/etc)

