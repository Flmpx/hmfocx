# Commit Format
This file will record how to write commit message.   


## Structure
Include `main`, `detail`, `reason` message part.   
Main message is must, but others is optional.  

### Main Message
Conclude this commit. 

#### Rule
- This part follow `conventional commit`(only head part) and the sentence shouldn't be too long.   
- Choose the most important message when this commit include many aspect.   
  
#### Example 
```txt
refactor!(list): Change the name of list's node  
```

   
### Detail Message 
Write out the specific content point by point. 

#### Rule
- This part have a break line with priviout part.  
- This part include ten `-`, `Detail: ` and detailed information.   
- The Detailed information start with `- ` and have to write it point by point.   
  
#### Example
```txt
----------
Detail:  
- The name:  `hm_listnode` -->  `hm_list_node`  
- Document and test code is changed according this refactor  
```

### Reason Message
Write out the reason of change these point by point. 

#### Rule
- This part's rule is similar as [Detail Message](#detail-message), only `Detail:` should be `Reason:`.  
  
#### Example
```txt
----------  
Reason:  
- Because map's entry is `hm_map_entry`, and set's entry is `hm_set_entry`, so, list's node should be `hm_list_node`  
```

### Next Message
Conclude next plan. 

#### Rule
- This part's rule is similar as [Detail Message](#detail-message), only `Detail:` should be `Next:`.  

#### Example
```txt
----------  
Next:  
- The free design of `hm_str` will apply in the other containers
```
