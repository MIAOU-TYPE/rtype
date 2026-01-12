---
id: auth-password-hashing
title: Password Hashing (scrypt)
sidebar_label: Password Hashing
---

# Password Hashing (server)

Passwords are hashed using **scrypt** via OpenSSL's `EVP_PBE_scrypt`.

Auth code:

- `server/src/auth/password/Password.*`

## Goals

- Never store plaintext passwords.
- Make offline password cracking expensive (memory-hard derivation).
- Store all required parameters alongside the hash, so verification is possible later.

## Hash function

### Parameters

The implementation uses:

- `N = 2^15` (CPU/memory cost)
- `r = 8` (block size)
- `p = 1` (parallelization)
- `saltLen = 16` bytes
- `dkLen = 32` bytes
- `maxMemBytes = 64 MiB`

### Encoding format

The hashed password is stored as a **single string**:

```
scrypt$<N>$<r>$<p>$<saltHex>$<dkHex>
```

Example (structure only):

```
scrypt$32768$8$1$<32 hex chars>$<64 hex chars>
```

- `saltHex` is the random salt (16 bytes => 32 hex chars)
- `dkHex` is the derived key (32 bytes => 64 hex chars)

## Hashing flow (`hashPassword`)

1. Generate a 16-byte salt using `RAND_bytes`.
2. Run `EVP_PBE_scrypt` with the parameters above.
3. Hex-encode salt and derived key.
4. Emit the encoded string.

Failure cases throw `PasswordError`.

## Verification flow (`verifyPassword`)

1. Parse the stored string:
   - checks prefix `scrypt$`
   - splits on `$` into 6 parts
2. Convert `N`, `r`, `p` and validate them.
3. Decode `saltHex` and `dkHex`.
4. Run `EVP_PBE_scrypt` with the parsed parameters.
5. Compare the computed derived key with the stored one using `constantTimeEqual`.

### Parameter validation rules

The implementation rejects hashes with unsafe or malformed parameters:

- `N` must be a power of two and `N >= 2^14`
- `r >= 8`
- `p >= 1`
- salt length must be between 8 and 64 bytes
- derived key length must be between 16 and 64 bytes

These checks prevent “crafted hash strings” from forcing pathological compute/memory settings.

## Security notes (current limits)

- Password transport is not encrypted at the protocol level (no TLS here). If the deployment is untrusted, wrap TCP in TLS or use a secure tunnel.
- There is no server-side rate limiting in the auth layer. If exposed to the internet, add IP-based throttling and/or account lockouts.
