# "My Storage", location-based warehousing (in development)

/--------------------------------------------------------------------------------------------/

Description.<br>
Simple application for location-based warehousing. It means, that you can store<br>
your goods in cells with numbers.<br>
- At first you'll be asked for inputs to generate cell numbers that fits your needs,<br>
in format "A-01-02", where 'A' is section,'01' is level, and '02' is number of the<br>
cell on that level. Fex. 5th level of section B with number 15 means "B-05-15".<br>
The rest of necessary tables will be created for you.<br>
- Then you can add items, incomes, outcomes and get the balance. Items can be<br>
filtered for convinient searching. Balance can be written to file (print.txt) for further<br>
printing.
As simple as it can be. Thats all for now, enjoy :)<br>

/--------------------------------------------------------------------------------------------/

Installing steps.<br>
- Run script init\_user\_database.sh for database and user creating.<br>
  ./init\_user\_database.sh_<br>
- Build project with 'make' and run it.<br>
  make && ./mystorage<br>

Further builds will not require runnig the script, so go ahead just with second step.

/--------------------------------------------------------------------------------------------/

<br>
![anim](https://github.com/user-attachments/assets/74fd794e-dea0-4dd3-a1b2-f0528756b2b9)
