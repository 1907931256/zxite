# SinkWorld project
# CxxTokens.py
# Tokenise C++ and provide methods to manipulate the tokenised form
# Copyright 2001 Neil Hodgson

keywords = [
"asm", "auto", "bool", "break", "case", "catch", "char", "class", "const",
"const_cast", "continue", "default", "delete", "do", "double",
"dynamic_cast", "else", "enum", "explicit", "export", "extern",
"false", "float", "for", "friend", "goto", "if", "inline", "int", "long",
"mutable", "namespace", "new", "operator", "private", "protected",
"public", "register", "reinterpret_cast", "return", "short", "signed",
"sizeof", "static", "static_cast", "struct", "switch", "template",
"this", "throw", "true", "try", "typedef", "typeid", "typename", "union",
"unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while",
# Invented keywords for the purpose of XLang
"SW_BYTE", "SW_SHORT", "SW_ABSTRACT", "SW_OVERRIDE", "SW_OBJECT", "SW_PUBLIC"
]

baseTypes = ["bool", "char", "double", "float", "int", 
	"long", "short", "signed", "unsigned", 
	"SW_BYTE", "SW_SHORT"]

# Encoding tokens as strings where the first charater is the token type
# and the remainder is the token value.
operators = "%^&*()-+=|{}[]:;<>,/?!.~#"
operatorPairs = ["%=", "^=", "&=", "*=", "-=", "+=", "==", "|=", "<=", ">=", "/=", "!=", 
"++", "--", "&&", "||", "<<", ">>", "->", ".*", "::", "##"]
operatorTrios = "->*", ">>=", "<<="
types = "cdiklmosw"	# Try to avoid C \ escapes so can be used together
# C uses abfnrtvx
comment = "c"
dqstring = "d"
identifier = "i"
keyword = "k"
commentline = "l"
number = "m"
operator = "o"
sqstring = "s"
whitespace = "w"
wildcard = "*"	# Not used in tokens - only for matching
codeTypes = "dikmos"

def codeAppend(l, tok):
	if tok:
		if tok[0] == "\"":
			l.append(dqstring + tok)
		elif tok[0] == "\'":
			l.append(sqstring + tok)
		elif tok.startswith("/*"):
			l.append(comment + tok)
		elif tok.startswith("//"):
			l.append(commentline + tok)
		elif tok[0] in operators:
			l.append(operator + tok)
		elif tok[0] in " \t":
			l.append(whitespace + tok)
		elif tok[0] in "0123456789":
			l.append(number + tok)
		elif tok in keywords:
			l.append(keyword + tok)
		else:
			l.append(identifier + tok)

def isWS(t):
	return t[0] in "wcl"
	
def substPositional(r, s, dropTypes=0):
	if type(r) == type(""):
		i = 0
		for b in s:
			if dropTypes:
				r = r.replace("$" + `i`, b[1:])
			else:
				r = r.replace("$" + `i`, b)
			i += 1
		return r
	else:	# List
		ret = []
		for t in r:
			t = substPositional(t, s)
			ret.append(t)
		return ret

def removeWS(s):
	ret = []
	for t in s:
		if not isWS(t):
			ret.append(t)
	return ret
	
# "" is used as a wildcard as no token is ever empty
# "*" is used as a wildcard type
def matchTokens(s, m):
	if s == m:
		return 1
	ks, vs = s[0], s[1:]
	km, vm = m[0], m[1:]
	valsMatch = vs == vm or vs == "" or vm == ""
	if ks == km and valsMatch:
		return 1
	if (ks == "*" or km == "*") and valsMatch:
		return 1
	return 0
	
def codeToTokens(s, inCommentStart):
	#print s
	inComment = inCommentStart
	ret = []
	current = ""
	prev = "%"
	inString = ""
	i = 0
	ch = ' '
	while i < len(s):
		ch = s[i]
		chNext = " "
		chNext2 = " "
		if i < len(s) - 1:
			chNext = s[i+1]
		if i < len(s) - 2:
			chNext2 = s[i+2]
		if inComment:
			current = current + ch
			if ch == '/' and prev == '*' and len(current) > 3:
				codeAppend(ret, current)
				current = ""
				inComment = ""
		elif inString:
			current = current + ch
			if ch == '\\':
				i += 1
				current = current + chNext
			elif ch == inString:
				codeAppend(ret, current)
				inString = ""
				current = ""
		else:
			if ch == "\"" or ch == "\'":
				codeAppend(ret, current)
				current = ch
				inString = ch
			elif ch in operators:
				codeAppend(ret, current)
				if i < len(s):
					if ch == "/" and chNext == "*":
						inComment = "*"
						current = ch
					elif ch == "/" and chNext == "/":
						inComment = "/"
						current = ch
					else:
						chPair = ch + chNext
						chTrio = ch + chNext + chNext2
						if chTrio in operatorTrios:
							codeAppend(ret, chTrio)
							i += 2
						elif chPair in operatorPairs:
							codeAppend(ret, chPair)
							i += 1
						else:
							codeAppend(ret, ch)
						current = ""
			elif ch in " \t":
				if current and not prev in " \t":
					codeAppend(ret, current)
					current = ""
				current = current + ch
			else:
				if current and prev in " \t":
					codeAppend(ret, current)
					current = ""
				current = current + ch
		prev = ch
		i += 1
	if current:
		codeAppend(ret, current)
	if inComment == "/":
		inComment = ""
		
	return ret, inComment
		
def matchTokenLists(s, m, ignoreWS=0):
	if type(m) == type(""):
		m = tokensFromStringForm(m)
	if len(m) > len(s):
		return 0
	si = 0
	mi = 0
	matchTokensLen = 0
	while mi < len(m) and si < len(s):
		if not matchTokens(m[mi], s[si]):
			return 0
		si += 1
		matchTokensLen = si
		while ignoreWS and si < len(s) and isWS(s[si]):
			si += 1
		mi += 1
	if mi < len(m):
		return 0
	return matchTokensLen

class CxxTokens:
	def __init__(self, tokens=[], inComment=None):
		self.inComment = inComment
		self.outComment = 0
		if type(tokens) == type(""):
			self.tokens, self.outComment = codeToTokens(tokens, inComment)
		else:
			self.tokens = tokens
			
	def __getitem__(self, index):
		return self.tokens[index]
		
	def matchTokenStrings(self, m, ignoreWS=0):
		return matchTokenLists(self.tokens, m, ignoreWS)

	def findTokenStrings(self, m, ignoreWS=0, startPos=0):
		if type(m) == type(""):
			m = tokensFromStringForm(m)
		if len(m) > (len(self.tokens)-startPos):
			return -1
		for pos in range(startPos, len(self.tokens) - len(m) + 1):
			if matchTokenLists(self.tokens[pos:], m, ignoreWS):
				return pos
		return -1
		
	def substTokenStrings(self, m, r, ignoreWS=0, startPos=0):
		if type(m) == type(""):
			m = tokensFromStringForm(m)
		if len(m) > len(self.tokens):
			return
		#print "matching", m
		pos = startPos
		while pos < len(self.tokens) - len(m) + 1:
			matchTokensLen = matchTokenLists(self.tokens[pos:], m, ignoreWS)
			if matchTokensLen:
				matchSection = self.tokens[pos:pos+matchTokensLen]
				if ignoreWS:
					matchSection = removeWS(matchSection)
				#print "match at", pos, matchTokensLen, "is", matchSection
				pre = CxxTokens(self.tokens[:pos])
				post = CxxTokens(self.tokens[pos+matchTokensLen:])
				toklen = len(self.tokens)
				if type(r) == type(""):
					self.tokens, self.outComment = codeToTokens(pre.flatten() + 
						substPositional(r, matchSection, 1) + post.flatten(), self.inComment)
				else:
					self.tokens = pre + substPositional(r, matchSection) + post
				pos += len(self.tokens) - toklen + 1
				#print "new val", self.tokens
			else:
				pos += 1
	
	def flatten(self):
		ret = ""
		for tok in self.tokens:
			ret += tok[1:]
		return ret
	
def tokensFromStringForm(s):
	ret = []
	current = ""
	i = 0
	while i < len(s):
		if s[i:].startswith("~~"):
			current += "~"
			i += 1
		elif s[i] == "~":
			ret.append(current)
			current = ""
		else:
			current += s[i]
		i += 1
	if current:
		ret.append(current)
	return ret
	
