def http_status(status):
    match status:
        case 200:
            return "ok"
        case 404:
            return "not found"
        case 500:
            return "internet server error"
        case _:
            return "unknown"
        
print(http_status(200))
print(http_status(404))
print(http_status(500))
print(http_status(600))
