 
how to use:
* download the plugin
* copy to your Plugins folder
* enable the plugin (run your project find it on the Plugins window and enable)
* then on your player begin play or some place that runs when the game start
* * find the Pooler subsystem in your nodes. (Get pooler subsystem)
* * call SetPool (with the bullet class)


* then whenever you need a bullet call Pooler->Get (passing the bullet class)


* when the bullet needs to die, instead of calling DestroyActor call Pooler->Return(self) (self or this) (self or this must be the bullet instance)

basically. then you can play with the other settings and parameters.


The pool will call SetActorHiddenInGame when you get a bullet and when you return it.
The pool will also call Reset on the bullet when you get one. So a trick is to override the "Reset" function in the bullet and reset values there.
