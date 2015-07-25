def changesets(base, mod):
    length = 0
    loc = 0

    for off, change in enumerate(zip(base, mod)):
        if change[0] == change[1]:
            if length:
                yield loc, length
                length = 0
        else:
            if length:
                length += 1
            else:
                # Start counting
                loc = off
                length = 1


def limit(changes, maxlen):
    print (list(changes))
    for change in changes:
        if change[1] > maxlen:
            yield change[0], maxlen
            change = (change[0] + maxlen, change[1] - maxlen)
        yield change


def ips(base, mod):
    # Get biggest filesize
    if len(base) != len(mod):
        raise ValueError('Files must be the same size')

    if len(base) > (2 ** 32):
        raise ValueError('Files are too large')

    # Check if we need 32 bit address space
    ips32 = len(base) > (2 ** 24)

    magic = 'IPS32' if ips32 else 'PATCH'
    yield magic.encode()

    # Look for changes
    changes = limit(changesets(base, mod), 0xFFFF)
    for change in changes:
        data = mod[change[0]:change[0] + change[1]]
        offset = change[0].to_bytes(4 if ips32 else 3, 'big')
        length = change[1].to_bytes(2, 'big')
        block = offset + length + data
        yield block
        
    footer = 'EOFF' if ips32 else 'EOF'
    yield footer.encode()
        

def make_patch(base_file, mod_file, patch_file):
    with open(base_file, 'rb') as base:
        with open(mod_file, 'rb') as modf:
            with open(patch_file, 'wb') as patch:
                base = base.read()
                modf = modf.read()
                for blob in ips(base, modf):
                    patch.write(blob)
    
