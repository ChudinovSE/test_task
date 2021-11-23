use solana_program::program_error::ProgramError;
use thiserror::Error;

#[derive(Error, Debug, Copy, Clone)]
pub enum ProcessingError {
    #[error("Not admin account. Acces denited!")]
    AccesDenited,
    #[error("Store not init now!")]
    StoreNotInit,
    #[error("Store is init now!")]
    StoreIsInit,
    
}

impl From<ProcessingError> for ProgramError {
    fn from(e: ProcessingError) -> Self {
        ProgramError::Custom(e as u32)
    }
}