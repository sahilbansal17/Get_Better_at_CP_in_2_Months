# Dynamic programming
> Those who cannot remember the past are condemned to repeat it.
> > Dynamic programming

The dynamic programming is about always remember answers to the sub-problems you've already solved.

The core idea of Dynamic programming is to avoid repeated work by remembering partial results and this concepts finds it application in a lot of real life situations.

In programming, Dynamic programming is a powerful technique that allows one to solve different types of problems in time O(n^2) or O(n^3) for which a naive approach would take exponential time.

Example: 3 * 15 = 3 * 10 + 3 * 5 or 1 + 1 + 1 + 1 = 5 + 1 = 6

The intuition behind dynamic programming is that we trade space for time.

In dynamic programming exist two great categories:
1. Optimization problems. Except you to select a feasible solution, so that the value of the required funtion is minimized or maximazed.
2. Combinatorial problems. Exect you to figure the number of ways to do something, or the posibility of some event happending.

Every dynamic programming problems has a schema to be followed:
- Show that the problem can be broken down into optimal sub-problems.
- Recursively define the value of the solution by expressing it in terms of optimal solutions smaller sub-problems.
- Compute the value of the optimal solution in button-up fashion.
- construct an optimal solution from the computed information.

**Bottom up vs Top down:**
- **Bottom Up.** I'm going to learn programming. Then I will start practicing. Then, I will start tacking part in contest. Then, I'll practice even more and try to improve. After working hard like a crazy, I'll be an amazing coder.
- **Top Down.** I will be an amazing coder. How? I will work hard like crazy. How? I'll practice more and try to improve. How? I'll start taking part in contests. Then? I'll practicing. How? I'm going to learn programming.

