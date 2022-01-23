# STV Voting Software

## :bulb: I n t r o d u c t i o n
</br>

### :question: What is it
> STV Voting System is a system of voting :sweat_smile:, when you are asked to fill in ballots not only with one single preferable candidate, but also order your preferences, starting with the top (most likely to choose). By doing latter, we give a chance for other ("weaker") candidates to be elected

### :earth_africa: Usage
> USA, UK, Australia, New Zealand, India, Pakistan, Ireland, Malta

### :key: What do I need to run the software
> Any C compiler (to simplify, use GCC for Unix-like and MinGw for Microsoft)
</br>

## :running: R u n

</br>

### :one: Clone *main* branch 

### :two: Type into terminal (only Unix-like, for Microsoft, please, lookup in the web):
```
gcc -c program.c utilities.c declarations.h
```
**Enter**
```
gcc program.o utilities.o declarations.h
```
**Enter**

### :three: You will be prompted to input values (more detailed usage prescriptions are shown on running time)

</br>

## :trollface: V u l n e r a b i l i t i e s

</br>

- [x] Usage of static arrays (we cannot dynamically expand boundaries and only restricted to the precise amount of people who vote and candidates who ballotate)
- [x] Usage of unstandartizied variable data types (int, char, etc). Each compiler can interpret them in a slightly different way
