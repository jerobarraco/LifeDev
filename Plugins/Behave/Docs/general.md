
this plugin is more something that i wanted to do for my own sake. if you need something powerful chuck ues state trees.
this might be useful for small projects, simpler ai, or for learning, or fun coding.

the design goal is to be clean, simple, and then flexible,

this is based on my own design, which is similar to  lgoap and state trees
truth is you can implement either with this plugin, just depending on how you choose to code it.

at first i wanted to make a strong distinctian between plan and action. then i realized
* you dont need to
* some planning requires actions
* there are some grey areas it can be a gamut
* dividng it will block me from being able to do certain things,
* itll complicate the architecture.

so this way you can
* have the flexibility to use it as lgoap or state trees
* maybe even as htn
* you can have lgoap if you keep your top nodes (a few levels) for plan only.

howto:
* add a datatable of BAction
  ** define the actions. actions can have other actions
* add a cbehave to your actor
  ** set ActionsToLoad to the row name in the dt of the root actions you want.
  ** the need to be sorted by priority fol goap, or use one single root action for state trees.
  ** See dyn priorities
* on the actor bind to OnState and do whatever you need there depending on the current action
* ive made it so that you can override most classes on bp
  ** specially the actions (the uobjects starting with B)
* you can use your own system for data. blackboards, global, your own subsystem, the owner actor, the action itself, your choice.
* there are some selectors like BPick, BRnd, BSeq, BFirst.
* there are some examplo implementations for the fish. like move, eat, play, moveat, etc. these are specific to this test. you will have to make your own. you can make them generic so you can reuse them in other projcets or make them specific and tightly coupled. im fine with both and wont judge you. as you can see these are tighly coupled. i could have made them generic but i didnt.

* the rest is documented on the code. look at Plan, DoSelf. those are the key ones.
* Plan runs on bg. DoSelf on main thread.
* Plans needs to return true if it can execute NOW
  ** make sure to set CostPlanned inside plan
  ** plan could be calledbut the action never started/chosen.
  ** see dyn priorities
  ** but ALSO it Must return false if it does not NEED to do it now. in that way you achieve dynamic priorities.
* on do
  ** make sure to return the appropiate value.
  ** do must return stop if the needed resoucre has ran out. or abort if there is an unplanned obstacle that can be avoided, or error.
* setstate
  ** make sure to initialize what you need here. you can do other initialization/deinit. if this is too lte, you can also do some on plan. but ideally plan should be light as possible.

Dynamic priorities
The architecture is somewhat clever and tries to do a few things.
theres a reason why cbehaves actions is a list. and expected to be sorted.
*
the order determines priority.
* this is also an optimization, as it wont even evaluate others.

* the very root actions define the interrupt priority. means task that WILL interrupt others. and that MUST be done over others. E.G escape > eat > attack. this is ALWAYS going to have thot priority, is just that sometimen you dont need to eat, or not in the need to escape. plan must return false if its not necessary to do, or not able to do. true if it can and wants.

The "tree" is quite dynamic. you can add and remove actions. and any action can create new actions and add as children or remove if they want (be sure to use the CBehave to "init" and "deinit" (aka register))

# optimization
move stuff away from these actions. like sensing.
have sense subsystem, the query from the actions.

WISHLIST
scopeguard on plan see georgy
* have modifiers / decotaros
* have roles /traits that can toggle actions
* params for actions
* passing data between actions (like on state trees)
  ** make it so that params are passing is the same struct and under themhood functionality. just different spot.
* keep checking the plan on the bg dn case something needs to be interrupted
  ** have a way to override the dnterruption by the user (e.g. a function)
* toggle root actions (move to another list)
* load action have index for insertion
* make actions private
* rename action to task, its shorter :)
* rename base B* to BT will that be confusing with behaviour tree ? sure.
* pass the row to the Action so that it can put its own id and stealwthe params from. it
* find a way to have variants on ue (ue surely has a base type for it) otherwise just a pointer to a struct. let the action cast it. and yes TVariant exists.
  https://georgy.dev/posts/variant/
  seems really limited. pointer to structs sounds way better.


notes
* i thought on making the actions structs instead of ubojecns but i think this gives more power, allow inheritance better on bp, and avoid data copying.
* the cbehave is compatible with my Significance plugin or tick interval throttling.

# Links

Slime rancher emergent storytelling.
https://www.youtube.com/watch?v=GbVFa89kUhw

See this one
https://www.youtube.com/watch?v=gm7K68663rA#
It's loosely based on the first person's talk.
And the comment about Layered GOAP.

Also look at state tree
it can do very similar stuff. and it's much cooler.
https://www.youtube.com/watch?v=YEmq4kcblj4
https://www.youtube.com/watch?v=zovPQnq7ndE

https://m.youtube.com/watch?v=Qq_xX1JCreI#
good video in bt, with good ideas for decorators
https://m.youtube.com/watch?v=uyYdOBYt6g4
non combat ai

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

