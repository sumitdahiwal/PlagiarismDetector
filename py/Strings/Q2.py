letter = ''' Dear <|Name|>,
            You are selected! 
            <|date|> '''
print(letter.replace("<|Name|>", "Sumit").replace("<|date|>", "24 Sept"))

