Yulun Zeng

I feel like there are actually 3 parts to MS1:
    Local storage allocation
    String collection
    Code generation

Local Storage Allocation:
First of all, I added some fields to the Symbol object to record data. A vreg int that records the vr number needed for operand generation. An unsigned addr to record the starting address of "special" variables.
I added two vectors in the LSA class, one is a Symbol vector, I think I totally could have used a Symbol table there, but both works. The other one is a Node vector.
    The symbol vector is to record symbols seen during visit declarator list. Then at the end of visit function definition, I will assign vr to each symbol sequentially. Now that I think about it again, I don't actually need a symbol vector, I can just reset the vr number at the end of the fucntion definition. 
    The Node vector is used for string collection. Whenever visiting a string literal, record the Node in the vector. After LSA, we can directly access the vector to create string constant label with module_collector. Since node.get_str() gives the entire string literal, there's no need to use the literal value class at all. In fact, if we use the literal value class's get string function, the string will come out distorted if there are things like \n. 
I did not use storage calculator. Instead, I created my own little storage calculator with some bitwise and math. The storage calculated will be used to annotate the variable starting address, it also automatically does padding. At the end of visit function definition, the cumulated storage will be used to annotate the function's address, which is actually the size used for enter and leave. 

String collection:
Already discussed above

Code generation:
This is too much work. But it's just rules to follow. There aren't many operations in nearly c. The most difficult parts are memory access for pointers, arrays. I am still confused when to use (vr). But I think I got it working anyway. The way I implemented might be a little better than how professor did it. Professor reset the temporary vr at the end of each expression. But to me, the temporary vr is only needed in certain circumstances, so after the vr is assigned to something, it's no longer needed, same for addition. I think this conforms to the actual x86 instruction. 
The pointer reference is the most work. Because there could be times when try to access pointer to pointer to pointer to...... And you cannot do something like ((((vr)))), so we need to break it down to several instruction, the idea being mov vr_temp (vr), mov vr_temp2 (vr_temp)... 
That's pretty much it. I just added some helpers, there are plenty of comments to help with my 4kb brain. 