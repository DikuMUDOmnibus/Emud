Hiyas,

Getting this baby running should be fairly easy, just go into the emud
directory, type ./compile, and ./emud.csh or ./startup when it's finished.
This should start the mud at port 4321. If it doesn't start up there might
be a file named RUNNING which you need to remove in order to boot Emud.
The source code should compile on Cygwin and most *nix systems.

I left 1 immortal character named Emud with a blank password. After logging in
you can change the name using: rename <newname> and set the password to
something appropriate using: fixpass self <password> <password>. All level 99
immortals need a vnum below 100, which at the moment needs to be changed
offline in the player file, the player.lst file which holds all the players
will likely update itself, though you might need to remove the old entry.

The documentation on the adminatration level of this codebase sucks which is
partly my mistake, but I can't be bothered fixing that at the moment. You'll
likely run into few issues with the OLC engine and mob progs. Emud is a very
stable codebase, with uptimes of up to three months. If you find any bugs
feel free to let me know.

I included a bunch of areas so you can get a hang of how mob progs can be used
with this codebase.

When running this mud you'd want to consider a few things at the design level.

The where command gives away part of the player's IP address. This completely
ruins the aspect of anonimity. For a social hack and slash mud that basicly
runs itself this wasn't entirely bad, but you might want to change this.

The who list shows what area the player is in. If you're going to allow PK I
strongly suggest removing this information from the who list.

Clans have backdoors which are leading to by the clan leader picked rooms
in the mud. This makes traveling a little too easy, either limitting or
removing backdoors would be an idea if you want to keep the illusion of
distance.

You'll also want to take a good look at the 'help rules' and adjust them to
your needs. Don't forget to use 'savearea help' when finished.

If you work on the code you'll want to set your tabwidth at 5 since it'll
look quite messy otherwise.

If you have questions, want to say hi, or let me know you run a mud based
on this code you can contact me at scandum@hotmail.com

I'll also try to keep a FAQ up to date at http://www.scandum.com/emud/

Igor