Yulun Zeng
For each optimization technique you implemented, your report should document

how the quality of the generated code was improved (include representative snippets of code before and after the optimization, for relevant test programs)
how the efficiency of the generated code improved (i.e., how much did performance of the benchmark programs improve)
Your report should also discuss what inefficiencies remain in the generated code, and what optimization techniques might be helpful for addressing them.

Please make a substantial effort to demonstrate improvements on “realistic” programs. As mentioned above, example29 and example31 are good candidates because they perform a substantial and realistic computation.

I started by copying the dead code elimination example. This optimization uses live variable analysis to eliminate some unnecessary operations in a block. Although this does not really affect anything since the example codes do not involve any dead codes, this optimization can be useful if there exists dead code. Overall, this optimization is minimal for the benchmark programs.

Then I started working on constant propagation. Rather than assigning constants to virtual registers, we can use constant values directly. As a result, I avoided moving a constant to a register. This is one of the biggest improvements I have made, it regularly eliminates 1 to 2 lines of code for each operation that involves constant values. For example31, this optimization reduced 42 lines of lower level x86 code.

For example 31, the speed after constant propagation approximately improves by 0.030s.
For example 29, the speed after constant propagation approximately improves by 0.100s.
