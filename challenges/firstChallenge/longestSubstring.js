let longestSubString = function(s){
  if(s.length == 1){
    return 1;
  }
  var characterSet = new Set()
  var longest = 0;
  for(var i = 0; i < s.length; i++){
    if(!(characterSet.has(s[i]))){
        characterSet.add(s[i]);
    }
    else{
      longest = characterSet.size >= longest ? characterSet.size : longest;
      characterSet.clear();
      characterSet.add(s[i]);
    }
  }
  console.log(s+" La subcadena mas grande tiene:");
  return characterSet.size >= longest ? characterSet.size : longest;

}


console.log(longestSubString('cielito'));
console.log(longestSubString('abcabcbb'));
console.log(longestSubString('bbbbbbb'));
console.log(longestSubString('aaaab'));
console.log(longestSubString('pwwkew'));


